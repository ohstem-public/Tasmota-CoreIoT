import string
import json
# List of devices to be sent via LoRa
var selected_devices = {}

# Function to handle user commands (add/remove device)
def handle_command(input_cmd)
  print("Received command: " + input_cmd)  # Debug command received
  
  var parts = string.split(input_cmd, " ")
  if parts.size() < 2
    print("Invalid command format!")
    return false
  end

  var cmd = parts[0]
  var device = parts[1]

  if cmd == "addDevice"
    print("Device added to LoRa: " + device)
    selected_devices[device] = true  # Save to LoRa device list
  elif cmd == "removeDevice"
    if selected_devices.find(device) != nil
      print("Device removed from LoRa: " + device)
      selected_devices.delete(device)
    else
      print("Device not found in the list!")
    end
  else
    print("Unknown command!")
  end
end

# Function to retrieve sensor data from Tasmota
def getsensors()
  var sensors = tasmota.read_sensors()  # Read sensor data
  var ressen = {}

  if sensors != nil && type(sensors) == 'instance'
    for entry: sensors.keys()
      if type(sensors[entry]) == 'instance' # If object, get sub-values
        for subentry: sensors[entry].keys()
          var key_name = entry+'-'+subentry
          if selected_devices.find(key_name) != nil
            ressen[key_name] = sensors[entry][subentry]
          end
        end
      end
    end
  end

  if ressen.size() > 0
    print("Sensor data: " + json.dump(ressen))
    return json.dump(ressen)
  else
    print("No selected sensor data available!")
    return nil
  end
end

# Function to display the list of selected devices
def list_devices()
  print("Selected devices for LoRa transmission:")
  for key: selected_devices.keys()
    print("- " + key)
  end
end

# Automatically run every 10 seconds to fetch sensor data
tasmota.add_cron("*/10 * * * * *", getsensors, "every_10_seconds")
