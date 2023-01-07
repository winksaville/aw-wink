#include <iostream>
#include <string>

#include <Wink/address.h>
#include <Wink/log.h>
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
  Machine m(spawner, address, "hierarchy/Simple", socket);

  m.AddState(std::make_unique<State>(
      // State Name
      "Parent",
      // Parent State
      "",
      // On Entry Action
      []() {},
      // On Exit Action
      []() {},
      // Receivers
      std::map<const std::string, Receiver>{
          {"",
           [&](const Address &sender, std::istream &args) {
             std::ostringstream os;
             os << args.rdbuf();
             info() << "Parent: " << os.str() << '\n' << std::flush;
           }},
      }));

  m.AddState(std::make_unique<State>(
      // State Name
      "Leaf1",
      // Parent State
      "Parent",
      // On Entry Action
      []() { info() << "Leaf1 On Entry\n"
                    << std::flush; },
      // On Exit Action
      []() {},
      // Receivers
      std::map<const std::string, Receiver>{
          {"Test",
           [&](const Address &sender, std::istream &args) {
             std::ostringstream os;
             os << args.rdbuf();
             info() << "Leaf1: " << os.str() << '\n' << std::flush;
           }},
      }));

  m.AddState(std::make_unique<State>(
      // State Name
      "Leaf2",
      // Parent State
      "Parent",
      // On Entry Action
      []() { info() << "Leaf2 On Entry\n"
                    << std::flush; },
      // On Exit Action
      []() {},
      // Receivers
      std::map<const std::string, Receiver>{
          {"Test",
           [&](const Address &sender, std::istream &args) {
             std::ostringstream os;
             os << args.rdbuf();
             info() << "Leaf2: " << os.str() << '\n' << std::flush;
           }},
      }));

  m.Start("Leaf2");
}