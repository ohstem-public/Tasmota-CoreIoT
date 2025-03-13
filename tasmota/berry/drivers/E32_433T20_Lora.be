import json
import string
# List of devices to be sent via LoRa
var selected_devices = {}
def SayHello()
  tasmota.cmd("Say_Hello")
end
# Function to retrieve sensor data from Tasmota
def getsensors()
 var sensors = json.load(tasmota.read_sensors())
 var ressen = {}
 if sensors != nil && type(sensors) == 'instance'
  for entry: sensors.keys() #check all entry in sensor data
      if type(sensors[entry]) == 'instance' #if instance list all subvalues
         for subentry: sensors[entry].keys()
             ressen[entry+'-'+subentry] = sensors[entry][subentry]
         end
      end
  end
 end
 if ressen.size()>0
#   return json.dump(ressen)
   return ressen
 else
  return nil
 end
end

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

  # Lấy danh sách cảm biến hiện tại trước khi xử lý lệnh
  var available_sensors = getsensors()
  if available_sensors == nil
    print("No sensors detected! Cannot add any device.")
    return false
  end

  if cmd == "addDevice"
   var found = False
   for list_device: available_sensors.keys()
      if list_device == device  # Kiểm tra cảm biến có tồn tại không
         print("Device added to LoRa: " + device)
         selected_devices[device] = true  # Save to LoRa device list
         found = True
      end
   end
   if found == False
      print("Error: Device not found in available sensors!")
      print("Available sensors: " + json.dump(available_sensors))
   end
  elif cmd == "removeDevice"
   for list_device: available_sensors.keys()
      if list_device == device  # Kiểm tra cảm biến có tồn tại không
         print("Remove to LoRa: " + device)
         selected_devices.delete(device)  #delete
      end
   end
   print("Error: Device not found in available sensors!")
   print(json.dump(available_sensors)) 
  else
    print("Unknown command!")
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
