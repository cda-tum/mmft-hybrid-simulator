#include "../src/baseSimulator.h"

#include "gtest/gtest.h"

using T = double;

/** Case 1:
 * 
 *  3 way node
 *  - 2 inflows
 *  - 1 outflow
*/
TEST(Topology, case1) {
    // define network
    arch::Network<T> network;

    // nodes
    auto node0 = network.addNode(0.0, 0.0, true);
    auto node1 = network.addNode(0.0, 2e-3, true);
    auto node2 = network.addNode(1e-3, 1e-3, false);
    auto node3 = network.addNode(2e-3, 1e-3, true);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;
    T flowRate = 3e-11;
    
    auto c0 = network.addChannel(node0->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c1 = network.addChannel(node1->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c2 = network.addChannel(node2->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    
    c0->setPressure(flowRate);
    c1->setPressure(0.5*flowRate);
    c2->setPressure(1.5*flowRate);

    c0->setResistance(1);
    c1->setResistance(1);
    c2->setResistance(1);

    // mixing model
    sim::DiffusionMixingModel<T> mixingModel;
    for (auto& [nodeId, node] : network.getNodes()) {
        mixingModel.topologyAnalysis(&network, nodeId);
    }
    mixingModel.printTopology();
}

/** Case 2:
 * 
 *  3 way node
 *  - 1 inflow
 *  - 2 outflows
*/
TEST(Topology, case2) {
    // define network
    arch::Network<T> network;

    // nodes
    auto node0 = network.addNode(0.0, 1e-3, true);
    auto node1 = network.addNode(1e-3, 1e-3, false);
    auto node2 = network.addNode(2e-3, 0.0, true);
    auto node3 = network.addNode(2e-3, 2e-3, true);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;
    T flowRate = 3e-11;
    
    auto c0 = network.addChannel(node0->getId(), node1->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c1 = network.addChannel(node1->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c2 = network.addChannel(node1->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    
    c0->setPressure(2*flowRate);
    c1->setPressure(flowRate);
    c2->setPressure(flowRate);

    c0->setResistance(1);
    c1->setResistance(1);
    c2->setResistance(1);

    // mixing model
    sim::DiffusionMixingModel<T> mixingModel;
    for (auto& [nodeId, node] : network.getNodes()) {
        mixingModel.topologyAnalysis(&network, nodeId);
    }
    mixingModel.printTopology();
}

/** Case 3:
 * 
 *  4 way node
 *  - 1 inflow
 *  - 3 outflows
*/
TEST(Topology, case3) {
    // define network
    arch::Network<T> network;

    // nodes
    auto node0 = network.addNode(0.0, 1e-3, true);
    auto node1 = network.addNode(1e-3, 1e-3);
    auto node2 = network.addNode(2e-3, 0.0, true);
    auto node3 = network.addNode(2e-3, 1e-3, true);
    auto node4 = network.addNode(2e-3, 2e-3, true);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;
    T flowRate = 3e-11;
    
    auto c0 = network.addChannel(node0->getId(), node1->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c1 = network.addChannel(node1->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c2 = network.addChannel(node1->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c3 = network.addChannel(node1->getId(), node4->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
  
    c0->setPressure(3*flowRate);
    c1->setPressure(0.5*flowRate);
    c2->setPressure(1.5*flowRate);
    c3->setPressure(flowRate);

    c0->setResistance(1);
    c1->setResistance(1);
    c2->setResistance(1);
    c3->setResistance(1);

    // mixing model
    sim::DiffusionMixingModel<T> mixingModel;
    for (auto& [nodeId, node] : network.getNodes()) {
        mixingModel.topologyAnalysis(&network, nodeId);
    }
    mixingModel.printTopology();
}

/** Case 4:
 * 
 *  4 way node
 *  - 2 adjacent inflows
 *  - 2 adjacent outflow
*/
TEST(Topology, case4) {
    // define network
    arch::Network<T> network;

    // nodes
    auto node0 = network.addNode(0.0, 0.0, true);
    auto node1 = network.addNode(0.0, 2e-3, true);
    auto node2 = network.addNode(1e-3, 1e-3, false);
    auto node3 = network.addNode(2e-3, 0.0, true);
    auto node4 = network.addNode(2e-3, 2e-3, true);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;
    T flowRate = 3e-11;
    
    auto c0 = network.addChannel(node0->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c1 = network.addChannel(node1->getId(), node2->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c2 = network.addChannel(node2->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c3 = network.addChannel(node2->getId(), node4->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    
    c0->setPressure(flowRate);
    c1->setPressure(flowRate);
    c2->setPressure(0.5*flowRate);
    c3->setPressure(1.5*flowRate);

    c0->setResistance(1);
    c1->setResistance(1);
    c2->setResistance(1);
    c3->setResistance(1);

    // mixing model
    sim::DiffusionMixingModel<T> mixingModel;
    for (auto& [nodeId, node] : network.getNodes()) {
        mixingModel.topologyAnalysis(&network, nodeId);
    }
    mixingModel.printTopology();
}

/** Case 5:
 * 
 *  4 way node
 *  - 3 inflows
 *  - 1 outflow
*/
TEST(Topology, case5) {
    // define network
    arch::Network<T> network;

    // nodes
    auto node0 = network.addNode(0.0, 0.0, true);
    auto node1 = network.addNode(0.0, 1e-3, true);
    auto node2 = network.addNode(0.0, 2e-3, true);
    auto node3 = network.addNode(1e-3, 1e-3, false);
    auto node4 = network.addNode(2e-3, 1e-3, true);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;
    T flowRate = 3e-11;
    
    auto c0 = network.addChannel(node0->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c1 = network.addChannel(node1->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c2 = network.addChannel(node2->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    auto c3 = network.addChannel(node3->getId(), node4->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    
    c0->setPressure(flowRate);
    c1->setPressure(0.5*flowRate);
    c2->setPressure(1.5*flowRate);
    c3->setPressure(2*flowRate);

    c0->setResistance(1);
    c1->setResistance(1);
    c2->setResistance(1);
    c3->setResistance(1);

    // mixing model
    sim::DiffusionMixingModel<T> mixingModel;
    for (auto& [nodeId, node] : network.getNodes()) {
        mixingModel.topologyAnalysis(&network, nodeId);
    }
    mixingModel.printTopology();
}

/** Case 6:
 * 
 *  4 way node (saddlepoint)
 *  - 2 inflows opposed
 *  - 2 outflows opposed
*/
// TEST(Topology, case6) {    
//     // define network
//     arch::Network<T> network;

//     // nodes
//     auto node0 = network.addNode(0.0, 0.0, true);
//     auto node1 = network.addNode(0.0, 1e-3, true);
//     auto node2 = network.addNode(0.0, 2e-3, true);
//     auto node3 = network.addNode(1e-3, 1e-3, false);
//     auto node4 = network.addNode(2e-3, 1e-3, true);

//     // channels
//     auto cWidth = 100e-6;
//     auto cHeight = 30e-6;
//     auto cLength = 1000e-6;
//     T flowRate = 3e-11;
    
//     auto c0 = network.addChannel(node0->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
//     auto c1 = network.addChannel(node3->getId(), node1->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
//     auto c2 = network.addChannel(node2->getId(), node3->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
//     auto c3 = network.addChannel(node3->getId(), node4->getId(), cHeight, cWidth, cLength, arch::ChannelType::NORMAL);
    
//     c0->setPressure(flowRate);
//     c1->setPressure(0.5*flowRate);
//     c2->setPressure(1.5*flowRate);
//     c3->setPressure(2*flowRate);

//     c0->setResistance(1);
//     c1->setResistance(1);
//     c2->setResistance(1);
//     c3->setResistance(1);

//     // mixing model
//     sim::DiffusionMixingModel<T> mixingModel;
//     for (auto& [nodeId, node] : network.getNodes()) {
//         mixingModel.topologyAnalysis(&network, nodeId);
//     }
//     mixingModel.printTopology();
// }

/** Case1:
 * 
 * operations 1 and 2
*/
TEST(DiffusionMixing, case1) {

// Define Simulation
sim::Simulation<T> testSimulation;
testSimulation.setType(sim::Type::_1D);
testSimulation.setPlatform(sim::Platform::MIXING);

// define network
arch::Network<T> network;
testSimulation.setNetwork(&network);

// nodes
auto node0 = network.addNode(1e-3, 0.0, false);
auto node1 = network.addNode(1e-3, 2e-3, false);
auto node2 = network.addNode(2e-3, 1e-3, false);
auto node3 = network.addNode(4e-3, 1e-3, false);
auto node4 = network.addNode(5e-3, 0.0, true);
auto node5 = network.addNode(5e-3, 2e-3, true);
auto node6 = network.addNode(0.0, 0.0, true);
auto node7 = network.addNode(0.0, 2e-3, true);

// channels
auto cWidth = 100e-6;
auto cHeight = 100e-6;
auto cLength = 1000e-6;
T flowRate = 3e-11;

auto c0 = network.addChannel(node0->getId(), node2->getId(), cHeight, cWidth, sqrt(2)*1e-3, arch::ChannelType::NORMAL);
auto c1 = network.addChannel(node1->getId(), node2->getId(), cHeight, cWidth, sqrt(2)*1e-3, arch::ChannelType::NORMAL);
auto c2 = network.addChannel(node2->getId(), node3->getId(), cHeight, cWidth, 2e-3, arch::ChannelType::NORMAL);
auto c3 = network.addChannel(node3->getId(), node4->getId(), cHeight, cWidth*2, sqrt(2)*1e-3, arch::ChannelType::NORMAL);
auto c4 = network.addChannel(node3->getId(), node5->getId(), cHeight, cWidth, sqrt(2)*1e-3, arch::ChannelType::NORMAL);
auto c5 = network.addPressurePump(node6->getId(), node0->getId(), 80);
auto c6 = network.addPressurePump(node7->getId(), node1->getId(), 80);

auto fluid1 = testSimulation.addFluid(1e-3, 1e3, 1.0);

testSimulation.setContinuousPhase(fluid1->getId());

sim::ResistanceModelPoiseuille<T> resistanceModel = sim::ResistanceModelPoiseuille<T>(testSimulation.getContinuousPhase()->getViscosity());
testSimulation.setResistanceModel(&resistanceModel);
sim::DiffusionMixingModel<T> diffusionMixingModel = sim::DiffusionMixingModel<T>();
testSimulation.setMixingModel(&diffusionMixingModel);
testSimulation.diffusiveMixing = true;

// Check if network is valid
network.isNetworkValid();
network.sortGroups();

T flowRateC2 = c2->getFlowRate();
T velocity = c2->getFlowRate() / c2->getArea();

auto specie1 = testSimulation.addSpecie(1e-8, 8.3);
std::unordered_map<int, T> species;
species.try_emplace(specie1->getId(), 1.0);
sim::DiffusiveMixture<T>* mixture = testSimulation.addDiffusiveMixture(species);
testSimulation.addMixtureInjection(mixture->getId(), c1->getId(), 0.0);

testSimulation.simulate();

std::cout << "Flow Rate: " << flowRateC2 << "\tVelocity:" << velocity << std::endl;

// results
// result::SimulationResult<T>* result = testSimulation.getSimulationResults();

}


// TEST(Mixing, Case1) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network1.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case1.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 1:
//      * 
//      * State 0 - Pre-Injection
//      * State 1 - Post-Injection
//      * State 2 - Mixture 0 reaches node 4
//      * State 3 - Mixture 1 reaches node 5
//     */

//     ASSERT_EQ(result->getStates().size(), 4);
//     ASSERT_EQ(result->getMixtures().size(), 2);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.912023, 5e-7);

//     ASSERT_EQ(result->getStates().at(0)->getMixturePositions().size(), 0);

//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().size(), 1);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(2).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().size(), 2);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(4).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(4).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().size(), 2);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(4).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(4).front().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 1);

//     ASSERT_NEAR(result->getMixtures().at(1)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0), 1e-7);
// }

// TEST(Mixing, Case2) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network1.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case2.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 2:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 reaches node 4 and creates Mixture 2
//      * State 4 - Mixture 2 reaches node 5
//      * State 5 - Mixture 1 reaches node 4 and creates Mixture 3
//      * State 6 - Mixture 3 reaches node 5
//     */

//     ASSERT_EQ(result->getStates().size(), 7);
//     ASSERT_EQ(result->getMixtures().size(), 4);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.500000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 0.912023, 5e-7);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 1.245356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 1.412023, 5e-7);

//     ASSERT_EQ(result->getStates().at(0)->getMixturePositions().size(), 0);

//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().size(), 1);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(2).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().size(), 2);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position2, 0.670820, 5e-7);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position2, 0.329180, 5e-7);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(4).front().mixtureId, 2);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(4).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(3).front().position2, 0.552786, 5e-7);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(4).front().mixtureId, 2);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(4).front().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(4).front().mixtureId, 3);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(4).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(4).back().mixtureId, 2);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(4).back().channel, 4);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(4).back().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(4).back().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(4).front().mixtureId, 3);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(4).front().channel, 4);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(4).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(4).front().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(2)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(3)->getSpecieConcentrations().size(), 1);

//     ASSERT_NEAR(result->getMixtures().at(2)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0), 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(3)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0) + 
//         0.5*result->getMixtures().at(1)->getSpecieConcentrations().at(0), 1e-7);
    
// }

// TEST(Mixing, Case3) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network2.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case3.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 3:
//      * 
//      * State 0 - Pre-Injection
//      * State 1 - Post-Injection
//      * State 2 - Mixture 0 reaches node 2
//      * State 3 - Mixture 0 reaches nodes 3 and 4
//     */

//     ASSERT_EQ(result->getStates().size(), 4);
//     ASSERT_EQ(result->getMixtures().size(), 1);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.666667, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 2.157379, 5e-7);

//     ASSERT_EQ(result->getStates().at(0)->getMixturePositions().size(), 0);

//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().size(), 1);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(1).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);

// }

// TEST(Mixing, Case4) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network2.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case4.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 4:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Mixture 0 reaches node 2
//      * State 3 - Mixture 0 reaches nodes 3 and 4
//      * State 4 - Post-Injection 2
//      * State 5 - Mixture 1 reaches node 2
//      * State 6 - Mixture 1 reaches node 3 and 4
//     */

//     ASSERT_EQ(result->getStates().size(), 7);
//     ASSERT_EQ(result->getMixtures().size(), 2);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.666667, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 2.157379, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 3.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 3.666667, 5e-7);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 5.157379, 5e-7);

//     ASSERT_EQ(result->getStates().at(0)->getMixturePositions().size(), 0);

//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().size(), 1);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(1)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getMixturePositions().at(1).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(2).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(2)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getMixturePositions().at(3).front().position2, 0.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(1).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(3)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(1).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(1).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(1).back().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(1).back().channel, 1);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(1).back().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(1).back().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(2).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(3).front().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(4)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(1).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).back().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(2).back().channel, 2);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).back().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(2).back().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).front().position2, 0.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).back().mixtureId, 0);
//     ASSERT_EQ(result->getStates().at(5)->getMixturePositions().at(3).back().channel, 3);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).back().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(5)->getMixturePositions().at(3).back().position2, 1.0, 1e-12);

//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().size(), 3);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(1).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(1).front().channel, 1);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(1).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(1).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(2).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(2).front().channel, 2);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(2).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(2).front().position2, 1.0, 1e-12);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(3).front().mixtureId, 1);
//     ASSERT_EQ(result->getStates().at(6)->getMixturePositions().at(3).front().channel, 3);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(3).front().position1, 0.0, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getMixturePositions().at(3).front().position2, 1.0, 1e-12);

// }

// TEST(Mixing, Case5) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network3.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case5.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 5:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Mixture 0 reaches node 2
//      * State 3 - Mixture 0 reaches node 4
//      * State 4 - Mixture 0 reaches nodes 3 and 5
//     */

//     ASSERT_EQ(result->getStates().size(), 5);
//     ASSERT_EQ(result->getMixtures().size(), 1);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.333333, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 2.192570, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 2.657379, 5e-7);

// }

// TEST(Mixing, Case6) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network3.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case6.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 6:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Mixture 0 reaches node 2
//      * State 3 - Post-Injection 2
//      * State 4 - Mixture 1 reaches node 2
//      * State 5 - Mixture 0 reaches node 4
//      * State 6 - Mixture 0 reaches node 3 and 5
//      * State 7 - Mixture 1 reaches node 4
//      * State 8 - Mixture 1 reaches node 3 and 5
//     */

//     ASSERT_EQ(result->getStates().size(), 9);
//     ASSERT_EQ(result->getMixtures().size(), 2);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.333333, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.500000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 0.833333, 5e-7);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 2.192570, 5e-7);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 2.657379, 5e-7);
//     ASSERT_NEAR(result->getStates().at(7)->getTime(), 2.692570, 5e-7);
//     ASSERT_NEAR(result->getStates().at(8)->getTime(), 3.157379, 5e-7);

// }

// TEST(Mixing, Case7) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network4.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case7.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 7:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 reaches node 4
//      * State 4 - Mixture 0 reaches node 5 and 6
//     */

//     ASSERT_EQ(result->getStates().size(), 5);
//     ASSERT_EQ(result->getMixtures().size(), 1);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 1.216761, 5e-7);

// }

// TEST(Mixing, Case8) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network4.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case8.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 8:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 and 1 reach node 4
//      * State 4 - Mixture 3 reaches node 5 and 6
//      * State 5 - Post-Injection 3
//      * State 6 - Mixture 2 reaches node 4
//      * State 7 - Mixture 4 reaches node 5 and 6
//     */

//     ASSERT_EQ(result->getStates().size(), 8);
//     ASSERT_EQ(result->getMixtures().size(), 5);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 1.216761, 5e-7);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 2.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 2.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(7)->getTime(), 3.216761, 5e-7);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 2);
//     ASSERT_EQ(result->getMixtures().at(2)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(3)->getSpecieConcentrations().size(), 2);
//     ASSERT_EQ(result->getMixtures().at(4)->getSpecieConcentrations().size(), 2);

//     ASSERT_NEAR(result->getMixtures().at(3)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0) +
//         0.5*result->getMixtures().at(1)->getSpecieConcentrations().at(0), 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(3)->getSpecieConcentrations().at(1), 
//         0.5*result->getMixtures().at(1)->getSpecieConcentrations().at(1), 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(4)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0), 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(4)->getSpecieConcentrations().at(1), 
//         0.5*result->getMixtures().at(2)->getSpecieConcentrations().at(1), 1e-7);
    

// }

// TEST(Mixing, Case9) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network5.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case9.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();
//     result->printLastState();

//     /**
//      * Case 9:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 reaches node 6
//      * State 4 - Mixture 1 reaches node 7
//      * State 5 - Mixture 0 reaches node 6
//      * State 6 - Mixture 2 reaches node 7
//      * State 7 - Post-Injection 3
//      * State 8 - Mixture 0 reaches node 6
//      * State 9 - Mixture 0 reaches node 7
//     */

//     ASSERT_EQ(result->getStates().size(), 10);
//     ASSERT_EQ(result->getMixtures().size(), 3);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.500000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 0.856467, 5e-7);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 1.166667, 5e-7);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 1.277778, 5e-7);
//     ASSERT_NEAR(result->getStates().at(7)->getTime(), 2.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(8)->getTime(), 2.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(9)->getTime(), 2.856467, 5e-7);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(2)->getSpecieConcentrations().size(), 1);

//     ASSERT_NEAR(result->getMixtures().at(1)->getSpecieConcentrations().at(0),
//         result->getMixtures().at(0)->getSpecieConcentrations().at(0)/3.0, 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(2)->getSpecieConcentrations().at(0),
//         result->getMixtures().at(0)->getSpecieConcentrations().at(0)/1.5, 1e-7);

// }

// TEST(Mixing, Case10) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network5.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case10.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 10:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 reaches node 6
//      * State 4 - Mixture 3 reaches node 7
//      * State 5 - Mixture 1 reaches node 6
//      * State 6 - Mixture 4 reaches node 7
//      * State 7 - Post-Injection 3
//      * State 8 - Mixture 2 reaches node 6
//      * State 9 - Mixture 5 reaches node 7
//     */

//     ASSERT_EQ(result->getStates().size(), 10);
//     ASSERT_EQ(result->getMixtures().size(), 6);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.500000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 0.856467, 5e-7);
//     ASSERT_NEAR(result->getStates().at(5)->getTime(), 1.166667, 5e-7);
//     ASSERT_NEAR(result->getStates().at(6)->getTime(), 1.277778, 5e-7);
//     ASSERT_NEAR(result->getStates().at(7)->getTime(), 2.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(8)->getTime(), 2.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(9)->getTime(), 2.856467, 5e-7);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 2);
//     ASSERT_EQ(result->getMixtures().at(2)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(3)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(4)->getSpecieConcentrations().size(), 2);
//     ASSERT_EQ(result->getMixtures().at(5)->getSpecieConcentrations().size(), 2);

//     ASSERT_NEAR(result->getMixtures().at(3)->getSpecieConcentrations().at(0), 
//         result->getMixtures().at(0)->getSpecieConcentrations().at(0)/3.0, 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(4)->getSpecieConcentrations().at(0), 
//         result->getMixtures().at(0)->getSpecieConcentrations().at(0)/3.0 +
//         result->getMixtures().at(1)->getSpecieConcentrations().at(0)/3.0, 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(4)->getSpecieConcentrations().at(1), 
//         result->getMixtures().at(1)->getSpecieConcentrations().at(1)/3.0, 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(5)->getSpecieConcentrations().at(0), 
//         result->getMixtures().at(0)->getSpecieConcentrations().at(0)/3.0 +
//         result->getMixtures().at(1)->getSpecieConcentrations().at(0)/3.0, 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(5)->getSpecieConcentrations().at(1), 
//         result->getMixtures().at(1)->getSpecieConcentrations().at(1)/3.0 +
//         result->getMixtures().at(2)->getSpecieConcentrations().at(1)/3.0, 1e-7);

// }

// TEST(Mixing, Case11) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network6.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case11.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 11:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Mixture 0 reaches node 4
//      * State 3 - Mixture 1 reaches nodes 5 and 6
//     */

//     ASSERT_EQ(result->getStates().size(), 4);
//     ASSERT_EQ(result->getMixtures().size(), 2);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 1.216761, 5e-7);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 1);

//     ASSERT_NEAR(result->getMixtures().at(1)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0), 1e-7);
// }

// TEST(Mixing, Case12) {
//     // Define JSON files
//     std::string networkFile = "../examples/1D/Mixing/Network6.JSON";
//     std::string simFile = "../examples/1D/Mixing/Case12.JSON";

//     // Load and set the network from a JSON file
//     arch::Network<T> network = porting::networkFromJSON<T>(networkFile);

//     // Load and set the simulations from the JSON files
//     sim::Simulation<T> sim = porting::simulationFromJSON<T>(simFile, &network);

//     // Check if network is valid
//     network.isNetworkValid();
//     network.sortGroups();

//     // simulate
//     sim.simulate();

//     // results
//     result::SimulationResult<T>* result = sim.getSimulationResults();

//     /**
//      * Case 12:
//      * 
//      * State 0 - Pre-Injections
//      * State 1 - Post-Injection 1
//      * State 2 - Post-Injection 2
//      * State 3 - Mixture 0 and 1 reach node 4
//      * State 4 - Mixture 2 reaches nodes 5 and 6
//     */

//     ASSERT_EQ(result->getStates().size(), 5);
//     ASSERT_EQ(result->getMixtures().size(), 3);

//     ASSERT_NEAR(result->getStates().at(0)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(1)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(2)->getTime(), 0.000000, 1e-12);
//     ASSERT_NEAR(result->getStates().at(3)->getTime(), 0.745356, 5e-7);
//     ASSERT_NEAR(result->getStates().at(4)->getTime(), 1.216761, 5e-7);

//     ASSERT_EQ(result->getMixtures().at(0)->getSpecieConcentrations().size(), 1);
//     ASSERT_EQ(result->getMixtures().at(1)->getSpecieConcentrations().size(), 2);
//     ASSERT_EQ(result->getMixtures().at(2)->getSpecieConcentrations().size(), 2);

//     ASSERT_NEAR(result->getMixtures().at(2)->getSpecieConcentrations().at(0), 
//         0.5*result->getMixtures().at(0)->getSpecieConcentrations().at(0) +
//         0.5*result->getMixtures().at(1)->getSpecieConcentrations().at(0), 1e-7);
//     ASSERT_NEAR(result->getMixtures().at(2)->getSpecieConcentrations().at(1), 
//         0.5*result->getMixtures().at(1)->getSpecieConcentrations().at(1), 1e-7);

// }