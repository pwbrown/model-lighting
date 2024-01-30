# [Model Lighting](../../README.md)/[Shared Libraries](../README.md)/MQTT Client

## Introduction
MqttClient is an abstracted class for connecting to an MQTT broker through WiFi, and subsbscring to topics, and publishing messages. It contains functions for checking the status of the connection, and for automatically recovering from failure.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**{repository_root}/{project_dir}/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/MqttClient
  symlink://../shared/Secrets
```

### WiFi credentials and MQTT host configuration

The MQTT client internally uses the [Secrets](../Secrets/README.md) library to manage connection details for WiFi and MQTT. Please refer to the Secrets documentation to setup credentials and connection details.

## Usage Examples

### Connecting to an MQTT Broker and listening for connection state

Using event listeners is completely optional, but can provide an easy way to visually report the status of the connection (ex. blink a light if the connection has dropped)

```cpp
#include <MqttClient.h>
#include <string>

// Create the client with a specific client id
MqttClient myClient("my_client_id");

// Create function to receive connecting events
void onConnectionUpdate(bool wifiOk, bool ipOk, bool mqttOk) {
  if (wifiOk && ipOk && mqttOk) {
    printf("MQTT Client is Connected\n");
  } else {
    printf("MQTT Client is Connecting...\n");
  }
}

void app_main(void) {
  myClient.configure()
    .onConnecting(onConnectionUpdate)
    .start();
}
```

### Subscribing to topics

```cpp
#include <MqttClient.h>
#include <string>

MqttClient myClient("my_client_id");

// This function will be called any time the
// "/my-project/print-message" topic has been published
void printMessage(std::string data) {
  printf("MQTT Message received: %s\n", data.c_str());
}

void app_main(void) {
  myClient.configure()
    .onTopic("/my-project/print-message", &printMessage)
    .start();
}
```

### Publishing to topics

Right now, publish calls will be ignored if the MQTT connection is inactive. In the future this may be converted to a queue system to allow messages in the queue to be published once the connection has become active again.

```cpp
#include <MqttClient.h>

MqttClient myClient("my_client_id");

void onConnectionUpdate(bool, bool, bool) {
  // This check is an alias for checking the
  // incoming 3 arguments if you don't need
  // a high degree of granularity
  if (client.isConnected()) {

    // Publish to the status topic when MQTT finishes connecting
    myClient.publish("/my-project/status", "connected");
  }
}

void app_main(void) {
  myClient.configure()
    .onConnecting(onConnectionUpdate)
    .start();
}
```

### Last Will and Testament (LWT)

The MQTT Last Will and Testament protocol is used to allow the MQTT broker to send one last message on the client's behalf if the client is ungracefully disconnected

```cpp
#include <MqttClient.h>

#define AVAILABLE_TOPIC "/my_client_id/available"

MqttClient myClient("my_client_id");

void onConnectionUpdate(bool, bool, bool) {
  if (client.isConnected()) {
    // Publish YES whenever the client connects
    myClient.publish(AVAILABLE_TOPIC, "YES");
  }
}

void app_main(void) {
  // Configure the client to publish NO as it's LWT
  myClient.configure(AVAILABLE_TOPIC, "NO", true)
    .onConnecting(onConnectionUpdate)
    .start();
}
```

## Member Functions

### `MqttClient(const char *clientId)` (constructor)

Create an MQTT client

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| const char* | clientId | The id that is assigned to this topic and made visible to the MQTT broker |

### `MqttClient &configure(string lwtTopic = "__NULL__", string lwtMsg = "__NULL__", bool lwtRetain = false)`

Configures the Non-Volatile Storage (NVS), WiFi connection, and MQTT Client. Additionally configures the MQTT Last Will and Testament (LWT) message.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| string | lwtTopic | The name of the LWT Topic | `"__NULL__"` |
| string | lwtMsg | The LWT message to send | `"__NULL__"` |
| bool | lwtRetain | Whether the MQTT broker should retain the LWT topic value | `false` |

### `MqttClient &onConnecting(function<void(bool, bool, bool)> callback)`

Registers a callback function for listening to connecting events.

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| function<void(bool, bool, bool)> | callback | Function that is called to report connecting status updates |

**Callback Parameters**
| Type | Name | Description |
| --- | --- | --- |
| bool | wifiOk | Indicates if the WiFi station is connected |
| bool | ipOk | Indicates if an IP address has been issued |
| bool | mqttOk | Indicates if the MQTT client is connected |

### `MqttClient &start(void)`

Starts connecting to WiFi and the MQTT client

### `bool isConnected(void)`

Indicates if the MQTT client is fully connected and ready to subscribe and publish to topics.

### `MqttClient &onTopic(string topic, function<void(string)> callback)`

Subscribes to an MQTT topic and registers a callback.

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| string | topic | The name of the MQTT topic to subscribe to |
| function<void(string)> | callback | Function that is called when a message on the topic is received |

**Callback Parameters**
| Type | Name | Description |
| --- | --- | --- |
| string | data | The data payload received on the topic |

### `MqttClient &publish(string topic, string data, bool retain = false)`

Subscribes to an MQTT topic and registers a callback.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| string | topic | The name of the MQTT topic to publish to | N/A |
| string | data | Data/payload to send with the published topic | N/A |
| bool | retain | Whether the MQTT broker should retain the topic value | `false` |
