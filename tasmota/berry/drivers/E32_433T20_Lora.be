import json
import string
# List of devices to be sent via LoRa
var list_selected_devices = {}
var lora_config = {
  "channel": 20,
  "addrHigh": 0x1,
  "addrLow": 0x2,
  "Baudrate": 3,
  "fixedTransmission": 0
}

# Set channel
def setChannel(val)
   lora_config["channel"] = val
   print(f"Channel is: {val}, please beginLora to set")
end

#SetAddrHigh   
def setAddrHigh(val)
  lora_config["addrHigh"] = val
  print(f"AddrHigh is: {val}, please beginLora to set")
end

#Set addrLow
def setAddrLow(val)
  lora_config["addrLow"] = val
  print(f"AddrLow is: {val}, please beginLora to set")
end

#Set baudrate
def setBaudrate(val)
  var baudrate_map = {
    "1200": 0,
    "2400": 1,
    "4800": 2,
    "9600": 3,
    "19200": 4,
    "38400": 5,
    "57600": 6,
    "115200": 7
  }

  var found = false
  for key: baudrate_map.keys()
    if string(key) == string(val)
      lora_config["Baudrate"] = baudrate_map[key]
      print(f"Baudrate set to {val} (code {baudrate_map[key]}), please beginLora to set")
      found = true
    end
  end

  if found == false
    print("Invalid baudrate! Valid options: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200")
  end
end



def setFixedTransmission(val)
  lora_config["fixedTransmission"] = val
  print(f"FixedTransmission is: {val}, please beginLora to set")
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
 for device:ressen.keys()
  if list_selected_devices.find(device) != nil
   list_selected_devices[device] = ressen[device]
  end
 end
 if ressen.size()>0
#   return json.dump(ressen)
   return ressen
 else
  return nil
 end
end

# Add telemetry you need
def addDevice(device)
  var available_sensors = getsensors()
  if available_sensors == nil
    print("No sensors detected! Cannot add any device.")
    return false
  end

  var found = false
  for list_device: available_sensors.keys()
    if list_device == device
      list_selected_devices[device] = available_sensors[device]
      print("Device added to LoRa: " + device)
      found = true
    end
  end

  if !found
    print("Error: Device not found in available sensors!")
    print("Available sensors: " + json.dump(available_sensors))
  end
end
#Remove telemetry you want to delete
def removeDevice(device)
  var available_sensors = getsensors()
  if available_sensors == nil
    print("No sensors detected! Cannot remove any device.")
    return false
  end

  var found = false
  for list_device: available_sensors.keys()
    if list_device == device
      list_selected_devices.remove(device)
      print("Removed device from LoRa: " + device)
      found = true
    end
  end

  if !found
    print("Error: Device not found in available sensors!")
  end
end

#Set up Lora
def beginLora()
  var json_str = json.dump(lora_config)
  print("Sending BeginLora config: " + json_str)
  tasmota.cmd("BeginLora " + json_str)
end
# Function to display the list of selected devices
def list_devices()
  print("Selected devices for LoRa transmission:")
  for key: list_selected_devices.keys()
    print("- " + key)
  end
end

def sendLoraBerry()
    getsensors()
    tasmota.cmd("SendLoraTelemetry " + json.dump(list_selected_devices))
end
# Automatically run every 10 seconds to fetch sensor data

tasmota.add_cron("*/10 * * * * *", sendLoraBerry, "every_10_seconds")
