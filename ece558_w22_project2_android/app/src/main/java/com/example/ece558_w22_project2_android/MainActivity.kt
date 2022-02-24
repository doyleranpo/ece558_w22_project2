package com.example.ece558_w22_project2_android

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.net.ConnectivityManager
import android.net.NetworkCapabilities
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.annotation.RequiresApi
import com.example.ece558_w22_project2_android.databinding.ActivityMainBinding
import com.google.android.material.slider.Slider
import org.eclipse.paho.client.mqttv3.*

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var mqttClient: MQTTClient
    private lateinit var mqttClientID: String

    @RequiresApi(Build.VERSION_CODES.M)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Check if Internet Connected
        if (!isConnected()) {
            Log.d("Internet","Not Connected")
            Toast.makeText(this, "Internet not available", Toast.LENGTH_LONG).show()
        } else {
            Log.d("Internet", "Connected")
            Toast.makeText(this, "Internet Connected", Toast.LENGTH_LONG).show()
        }
        // Create MQTT Broker Communication
        mqttClientID = MqttClient.generateClientId()
        mqttClient = MQTTClient(this, MQTT_SERVER_URI, mqttClientID)

        // Initial state of button and sliders
        binding.connect.isEnabled = true
        binding.disconnect.isEnabled = false
        binding.ledSlider.isEnabled = false
        binding.intervalSlider.isEnabled = false
        // Initial message for button
        binding.buttonstat.text = "Button Unknown"

        // Connect Listener for button
        binding.connect.setOnClickListener{
            // Connect to MQTT
            mqttClient.connect(
                MQTT_USERNAME,
                MQTT_PWD,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        // Disable connect Buttons
                        binding.connect.isEnabled = false
                        // Enable disconnect buttons
                        binding.disconnect.isEnabled = true

                        // Subscribe to the topics
                        subscribeDev()
                        // Enable sliders
                        binding.ledSlider.isEnabled = true
                        binding.intervalSlider.isEnabled = true

                        binding.buttonstat.text = "Button Released"
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                        val failureMsg = "MQTT Connection to $MQTT_SERVER_URI failed: ${exception?.toString()}"
                        Toast.makeText(this@MainActivity, failureMsg, Toast.LENGTH_LONG).show()
                        Log.d("MQTT Connection", failureMsg)
                    }
                },
                // Setup callback for MQTT
                object: MqttCallback {
                    override fun messageArrived(topic: String?, message: MqttMessage?) {
                        Log.d("MQTT Message", "Received message")

                        // Update text fields once the message arrives
                        if (topic!! == TEMP_TOPIC) {
                            binding.temp.text = message.toString()
                        }

                        if (topic == HUMID_TOPIC) {
                            binding.humid.text = message.toString()
                        }

                        if (topic == BUTTON_TOPIC) {
                            val statusButton = when {
                                message.toString().toInt() == 1 -> "Button Pressed"
                                else -> "Button Released"
                            }
                            binding.buttonstat.text = statusButton.toString()
                        }
                    }

                    override fun connectionLost(cause: Throwable?) {
                        Log.d("MQTT Message", "Connection lost ${cause.toString()}")
                    }

                    override fun deliveryComplete(token: IMqttDeliveryToken?) {
                        Log.d("MQTT Message", "Delivery complete")
                    }
                })
        }
        // Disconnect button implementation
        binding.disconnect.setOnClickListener{
            if (mqttClient.isConnected()) {
                mqttClient.disconnect(object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        Log.d("MQTT", "Disconnected from $$MQTT_SERVER_URI")
                        Toast.makeText(this@MainActivity, "MQTT Disconnection success", Toast.LENGTH_LONG).show()
                        // Enable connect button
                        binding.connect.isEnabled = true
                        // Disable every other entry
                        binding.disconnect.isEnabled = false
                        binding.ledSlider.isEnabled = false
                        binding.intervalSlider.isEnabled = false

                        binding.buttonstat.text = "Button Unknown"
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                        Log.d("MQTT", "Failed to disconnect exception: ${exception.toString()}")
                    }
                })
            } else {
                Log.d("MQTT", "Impossible to disconnect, no server connected")
            }
        }
        // Slider implementation for intervals
        binding.intervalSlider.addOnSliderTouchListener(object: Slider.OnSliderTouchListener {
            @SuppressLint("RestrictedApi")
            override fun onStartTrackingTouch(slider: Slider) {
                val interval = slider.value.toInt().toString()
                Log.d("IntervalSlider", interval)
            }

            @SuppressLint("RestrictedApi")
            override fun onStopTrackingTouch(slider: Slider) {
                val interval = slider.value.toInt().toString()
                Log.d("Interval Slider final", interval)
                changeInterval(interval)
            }
        })

        // Slider implementation for LED via PWM on Huzzah
        binding.ledSlider.addOnSliderTouchListener(object: Slider.OnSliderTouchListener {
            @SuppressLint("RestrictedApi")
            override fun onStartTrackingTouch(slider: Slider) {
                // Read the value and log
                val ledVal = slider.value.toInt().toString()
                Log.d("LED value", ledVal)
            }

            @SuppressLint("RestrictedApi")
            override fun onStopTrackingTouch(slider: Slider) {
                // Read the value on stop tracking
                val ledVal = slider.value.toInt().toString()
                Log.d("LED Value final", ledVal)
                // Publish the value to the topic
                changeLED(ledVal)
            }
        })

    }
    // Check connection to the right services
    @RequiresApi(Build.VERSION_CODES.M)
    private fun isConnected(): Boolean {
        var result = false
        val cm = getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
        val capabilities = cm.getNetworkCapabilities(cm.activeNetwork)
        if (capabilities != null) {
            result = when {
                capabilities.hasTransport(NetworkCapabilities.TRANSPORT_WIFI) ||
                        capabilities.hasTransport(NetworkCapabilities.TRANSPORT_CELLULAR) ||
                        capabilities.hasTransport(NetworkCapabilities.TRANSPORT_VPN) -> true
                else -> false
            }
        }
        return result
    }
    // Subscription topics for MQTT parameters
    private fun subscribeDev() {
        if (mqttClient.isConnected()) {
            // Subscribe to topics needed for functioning
                // Subscribe to temperature
            mqttClient.subscribe(
                topic = TEMP_TOPIC,
                qos = 1,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        Toast.makeText(this@MainActivity, "Subscribed to $TEMP_TOPIC", Toast.LENGTH_SHORT).show()
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                            Log.d(
                                "MQTT", "Failed to sub"
                            )
                      }
                    })
                // Subscribe to humidity
            mqttClient.subscribe(
                topic = HUMID_TOPIC,
                qos = 1,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        Toast.makeText(this@MainActivity, "Subscribed to $HUMID_TOPIC", Toast.LENGTH_SHORT).show()
                    }

                    override fun onFailure(
                        asyncActionToken: IMqttToken?,
                        exception: Throwable?
                    ) {
                        Log.d(
                            "MQTT", "Failed to sub"
                        )
                    }
                })
                // Subscribe to button status
            mqttClient.subscribe(
                topic = BUTTON_TOPIC,
                qos = 1,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        Toast.makeText(this@MainActivity, "Subscribed to $BUTTON_TOPIC", Toast.LENGTH_SHORT).show()
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                        Log.d("MQTT", "Failed to sub")
                    }
                })
        } else {
            Log.d("MQTT", "Failed to sub")
        }
    }

    private fun changeInterval(interval: String) {
        if(mqttClient.isConnected()) {
            val topic = INTERVAL_TOPIC
            // Publish to topic the value received from the slider
            mqttClient.publish(
                topic,
                interval,
                1,
                false,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        val msg = "Successfully published message to topic: $topic"
                        Log.d("MQTT Pub", msg)
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                        val msg = "Failed to publish: to topic: $topic exception: ${exception.toString()}"
                        Log.d("MQTT Pub", msg)
                    }
                })
        } else {
            Log.d("MQTT Pub", "Impossible to publish, no server connected")
        }
    }

    private fun changeLED(value: String) {
        if(mqttClient.isConnected()) {
            val topic = LED_TOPIC
            // Publish LED value obtained from Slider to MQTT
            mqttClient.publish(
                topic,
                value,
                1,
                false,
                object : IMqttActionListener {
                    override fun onSuccess(asyncActionToken: IMqttToken?) {
                        val msg = "Successfully published message to topic: $topic"
                        Log.d("MQTT Pub", msg)
                    }

                    override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                        val msg = "Failed to publish: to topic: $topic exception: ${exception.toString()}"
                        Log.d("MQTT Pub", msg)
                    }
                })
        } else {
            Log.d("MQTT Pub", "Impossible to publish, no server connected")
        }
    }
}