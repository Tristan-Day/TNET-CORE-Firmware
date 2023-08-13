#include <service/Console.hpp>

const string Console::SERVICE_UUID = "76b20df8-d995-43d8-b00d-f470a88e1526";

const string Console::INPUT_UUID = "7bce8d3f-c993-41d9-ac88-51ad2625ed69";
const string Console::OUTPUT_UUID = "98cc8ed4-bdbe-4360-a127-1d45822fec98";

Console::Console()
{
    inputCharacteristic =
        new BLECharacteristic(INPUT_UUID, BLECharacteristic::PROPERTY_WRITE);

    outputCharacteristic =
        new BLECharacteristic(OUTPUT_UUID, BLECharacteristic::PROPERTY_READ);

    service = Bluetooth::get()->pServer->createService(SERVICE_UUID);

    service->addCharacteristic(inputCharacteristic);
    inputCharacteristic->addDescriptor(new BLE2902);
    inputCharacteristic->setCallbacks(this);

    service->addCharacteristic(outputCharacteristic);
    outputCharacteristic->addDescriptor(new BLE2902);

    service->start();
}

void Console::onWrite(BLECharacteristic* characteristic)
{
    string input = characteristic->getValue();

#ifdef DEBUG
    Serial.println("[INFO] Console Write Callback");
#endif

    vector<string> components;
    string buffer;

    for (uint32_t i = 0; i < input.size(); i++)
    {
        if (input.at(i) == DELIMITER)
        {
            if (buffer.empty()) { continue; }

            components.push_back(buffer);
            buffer.clear();
        }
        else { buffer.push_back(input.at(i)); }
    }

    if (!buffer.empty()) { components.push_back(buffer); }

    string result = "Unknown Function";

    if (components.size() == 1)
    {
        auto runnable = runnables.find(components.at(0));

        if (runnable != runnables.end()) { result = runnable->second(); }
    }

    else if (components.size() == 2)
    {
        auto consumer = consumers.find(components.at(0));

        if (consumer != consumers.end())
        {
            result = consumer->second(components.at(1));
        }
    }

    outputCharacteristic->setValue(result);
}

void Console::addCommand(string name, function<string()> function)
{
    runnables[name] = function;
}

void Console::addCommand(string name, function<string(string)> function)
{
    consumers[name] = function;
}