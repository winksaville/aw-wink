#include <chrono>
#include <iostream>
#include <string>

#include <Wink/address.h>
#include <Wink/machine.h>
#include <Wink/state.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    error() << "Incorrect parameters, expected <spawner> <address>\n"
            << std::flush;
    return -1;
  }

  Address spawner(argv[1]);
  Address address(argv[2]);
  UDPSocket socket;
  Machine m(spawner, address, "time/After", socket);

  m.AddState(std::make_unique<State>(
      // State Name
      "main",
      // Parent State
      "",
      // On Entry Action
      [&]() {
        // Schedule message to be sent to self after 10s
        m.SendAfter(address, "exit", std::chrono::seconds(10));
      },
      // On Exit Action
      []() {},
      // Receivers
      std::map<const std::string, Receiver>{
          {"exit",
           [&](const Address &sender, std::istream &args) { m.Exit(); }},
      }));

  m.Start();
}