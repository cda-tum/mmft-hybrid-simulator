#include "MixingModels.h"

#include <unordered_map>
#include <deque>
#include <iostream>

#define M_PI 3.14159265358979323846

namespace sim {

template<typename T>
MixingModel<T>::MixingModel() { }

template<typename T>
T MixingModel<T>::getMinimalTimeStep() {
    return this->minimalTimeStep;
}

template<typename T>
InstantaneousMixingModel<T>::InstantaneousMixingModel() { }

template<typename T>
void InstantaneousMixingModel<T>::updateMixtures(T timeStep, arch::Network<T>* network, Simulation<T>* sim, std::unordered_map<int, std::unique_ptr<Mixture<T>>>& mixtures) {

    generateNodeOutflow(sim, mixtures);
    updateChannelInflow(timeStep, network, mixtures);
    clean(network);
    updateMinimalTimeStep(network);
}

template<typename T>
void InstantaneousMixingModel<T>::updateNodeInflow(T timeStep, arch::Network<T>* network) {

    for (auto& [nodeId, node] : network->getNodes()) {
        bool generateInflow = false;
        int totalInflowCount = 0;
        int mixtureInflowCount = 0;
        if (createMixture.count(nodeId)) {
            createMixture.at(nodeId) = false;
        } else {
            createMixture.try_emplace(nodeId, false);
        }
        for (auto& [channelId, channel] : network->getChannels()) {
            if ((channel->getFlowRate() > 0.0 && channel->getNodeB() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeA() == nodeId)) {
                totalInflowCount++;
                T inflowVolume = std::abs(channel->getFlowRate()) * timeStep;
                T movedDistance = inflowVolume / channel->getVolume();
                auto [iterator, inserted] = totalInflowVolumeAtNode.try_emplace(nodeId, inflowVolume);
                if (!inserted) {
                    iterator->second += inflowVolume;
                }
                if (mixturesInEdge.count(channel->getId())){
                    for (auto& [mixtureId, endPos] : mixturesInEdge.at(channel->getId())) {
                        T newEndPos = std::min(endPos + movedDistance, 1.0);
                        endPos = newEndPos;
                        if (newEndPos == 1.0) {
                            // if the mixture front left the channel, it's fully filled
                            if (filledEdges.count(channel->getId())) {
                                filledEdges.at(channel->getId()) = mixtureId;
                            } else {
                                filledEdges.try_emplace(channel->getId(), mixtureId);
                            }
                            generateInflow = true;
                        }   
                    }
                }
            }
        }
        // specie flows into node, add to mixture inflow
        if (generateInflow) {
            mixtureInflowCount = generateInflows(nodeId, timeStep, network);
        }
        if (totalInflowCount != mixtureInflowCount) {
            createMixture.at(nodeId) = true;
        }
    }
    for (auto& [nodeId, mixtureInflowList] : mixtureInflowAtNode) {
        for (auto& mixtureInflow : mixtureInflowList) {
            if (mixtureInflow.mixtureId != mixtureInflowList[0].mixtureId) {
                createMixture.at(nodeId) = true;
            }
        }
    }
}

template<typename T>
int InstantaneousMixingModel<T>::generateInflows(int nodeId, T timeStep, arch::Network<T>* network) {
    int mixtureInflowCount = 0;
    for (auto& [channelId, channel] : network->getChannels()) {
        T inflowVolume = std::abs(channel->getFlowRate()) * timeStep;
        if ((channel->getFlowRate() > 0.0 && channel->getNodeB() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeA() == nodeId)) {
            if (filledEdges.count(channelId)  && !network->getNode(nodeId)->getSink()) {
                MixtureInFlow<T> mixtureInflow = {filledEdges.at(channelId), inflowVolume};
                mixtureInflowAtNode[nodeId].push_back(mixtureInflow);
                mixtureInflowCount++;
            }
        }
    }
    return mixtureInflowCount;
}

template<typename T>
void InstantaneousMixingModel<T>::generateNodeOutflow(Simulation<T>* sim, std::unordered_map<int, std::unique_ptr<Mixture<T>>>& mixtures) {

    for (auto& [nodeId, mixtureInflowList] : mixtureInflowAtNode) {
        std::unordered_map<int, T> newConcentrations;
        for (auto& mixtureInflow : mixtureInflowList) {
            for (auto& [specieId, oldConcentration] : mixtures.at(mixtureInflow.mixtureId)->getSpecieConcentrations()) {
                T newConcentration = oldConcentration * mixtureInflow.inflowVolume / totalInflowVolumeAtNode.at(nodeId);
                auto [iterator, inserted] = newConcentrations.try_emplace(specieId, newConcentration);
                if (!inserted) {
                    iterator->second = iterator->second + newConcentration;
                }
            }
        }
        if ( !createMixture.at(nodeId)) {
            mixtureOutflowAtNode.try_emplace(nodeId, mixtureInflowList[0].mixtureId);
        } else {
            Mixture<T>* newMixture = sim->addMixture(newConcentrations);
            mixtureOutflowAtNode.try_emplace(nodeId, newMixture->getId());
            createMixture.at(nodeId) = false;
        }
    }
}

template<typename T>
void InstantaneousMixingModel<T>::updateChannelInflow(T timeStep, arch::Network<T>* network, std::unordered_map<int, std::unique_ptr<Mixture<T>>>& mixtures) {

    for (auto& [nodeId, node] : network->getNodes()) {
        for (auto& channel : network->getChannelsAtNode(nodeId)) {
            // check if edge is an outflow edge to this node
            if ((channel->getFlowRate() > 0.0 && channel->getNodeA() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeB() == nodeId)) {
                if (mixtureOutflowAtNode.count(nodeId)) {
                    injectMixtureInEdge(mixtureOutflowAtNode.at(nodeId), channel->getId());
                }
            }
        }
    }
}

template<typename T>
void InstantaneousMixingModel<T>::clean(arch::Network<T>* network) {
    
    for (auto& [nodeId, node] : network->getNodes()) {
        for (auto& channel : network->getChannelsAtNode(nodeId)) {
            if (mixturesInEdge.count(channel->getId())){
                for (auto& [mixtureId, endPos] : mixturesInEdge.at(channel->getId())) {
                    if (endPos == 1.0) {
                        mixturesInEdge.at(channel->getId()).pop_front();
                    } else {
                        break;
                    }
                }
            }
        }
    }
    mixtureInflowAtNode.clear();
    mixtureOutflowAtNode.clear();
    totalInflowVolumeAtNode.clear();
}

template<typename T>
void InstantaneousMixingModel<T>::updateMinimalTimeStep(arch::Network<T>* network) {
    this->minimalTimeStep = 0.0;
    for (auto& [channelId, deque] : mixturesInEdge) {
        T channelVolume = network->getChannel(channelId)->getVolume();
        T channelFlowRate = network->getChannel(channelId)->getFlowRate();
        for (auto& [mixtureId, endPos] : deque) {
            T flowTime = (1.0 - endPos)*channelVolume/channelFlowRate;
            if (this->minimalTimeStep < 1e-12) {
                this->minimalTimeStep = flowTime;
            } else if (flowTime < this->minimalTimeStep) {
                this->minimalTimeStep = flowTime;
            }
        }
    }
}

template<typename T>
void InstantaneousMixingModel<T>::injectMixtureInEdge(int mixtureId, int channelId) {
    if (mixturesInEdge.count(channelId)) {
        mixturesInEdge.at(channelId).push_back(std::make_pair(mixtureId, T(0.0)));
    } else {
        std::deque<std::pair<int,T>> newDeque;
        newDeque.push_back(std::make_pair(mixtureId, T(0.0)));
        mixturesInEdge.try_emplace(channelId, newDeque);
    }
}

template<typename T>
void InstantaneousMixingModel<T>::printMixturesInNetwork() {
    for (auto& [channelId, deque] : mixturesInEdge) {
        for (auto& [mixtureId, endPos] : deque) {
            std::cout << "Mixture " << mixtureId << " in channel " << channelId << 
            " at position " << endPos << std::endl;
        }
    }
}

template<typename T>
const std::deque<std::pair<int,T>>& InstantaneousMixingModel<T>::getMixturesInEdge(int channelId) const {
    return mixturesInEdge.at(channelId);
}

template<typename T>
const std::unordered_map<int, std::deque<std::pair<int,T>>>& InstantaneousMixingModel<T>::getMixturesInEdges() const {
    return mixturesInEdge;
}

template<typename T>
const std::unordered_map<int, int>& InstantaneousMixingModel<T>::getFilledEdges() const {
    return filledEdges;
}


//=========================================================================================
//================================  diffusive Mixing  =====================================
//=========================================================================================

template<typename T>
DiffusionMixingModel<T>::DiffusionMixingModel() : MixingModel<T>() { }

template<typename T>
void DiffusionMixingModel<T>::updateDiffusiveMixtures(T timeStep, arch::Network<T>* network, Simulation<T>* sim, std::unordered_map<int, std::unique_ptr<DiffusiveMixture<T>>>& mixtures) {
    updateNodeInflow(timeStep, network, mixtures)
    updateMinimalTimeStep(network);
}

template<typename T>
void DiffusionMixingModel<T>::updateNodeInflow(T timeStep, arch::Network<T>* network, std::unordered_map<int, std::unique_ptr<DiffusiveMixture<T>>>& mixtures) {
    for (auto& [nodeId, node] : network->getNodes()) {
        bool generateInflow = false;
        for (auto& [channelId, channel] : network->getChannels()) {
            // If the channel flows into this node
            if ((channel->getFlowRate() > 0.0 && channel->getNodeB() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeA() == nodeId)) {
                for (auto& [mixtureId, endPos] : mixturesInEdge.at(channel->getId())) {
                    // Propage the mixture positions in the channel with movedDistance
                    T newEndPos = std::min(endPos + movedDistance, 1.0);
                    endPos = newEndPos;
                    if (newEndPos == 1.0) {
                        // We must generate the outflow of this node, i.e., the inflow of the channels that flow out of the node
                        generateInflow = true;
                    }   
                }
            }
        }
        // Due to the nature of the diffusive mixing model, per definition a new mixture is created.
        // It is unlikely that this exact mixture, with same species and functions already exists
        if (generateInflow) {
            generateInflows(nodeId, timeStep, network, mixtures);
        }
    }
}

template<typename T>
void DiffusionMixingModel<T>::generateInflows(int nodeId, T timeStep, arch::Network<T>* network, std::unordered_map<int, std::unique_ptr<DiffusiveMixture<T>>>& mixtures) {
    topologyAnalysis();
    for (auto& [channelId, channel] : network->getChannels()) {
        // If the channel flows out of this node
        if ((channel->getFlowRate() > 0.0 && channel->getNodeA() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeB() == nodeId)) {
            // We should evaluate the final function of the mixture reaching the end of this channel
            for (auto& [specieId, specie_ptr] : mixtures.at(TODO)->getSpecieDistributions()) {
                T pecletNr = channel->getFlowRate() * channel->getWidth() / species->getDiffusionCoefficient();
                std::function<T(T)> concentrationAtChannelEndFunction = getAnalyticalSolutionTotal(channel->getChannelLength(), channel->getChannelWidth(), this->resolution, pecletNr, outflowDistributions.at(channel->getId()));
            }
        }
    }
}

template<typename T>
void DiffusionMixingModel<T>::topologyAnalysis( arch::Network<T>* network ) {
    /**
     * 1. List all connecting angles with angle and inflow
     * 2. Order connected channels according to radial angle
     * 3. Group connected channels into groups of 
     *      - Concatenated inflows
     *      - Concatenated outflows
     * 4. Inflow groups always (?) merge incoming channels
     * 5. Inflow group is always (?) split into the 1 or 2 neighbouring outflow groups
     * 6. Outflow groups always (?) merge the incoming 1 or 2 neighbouring inflow groups
     * 7. Outflow groups always (?) split the total income into channels.
    */
    for (auto& [nodeId, node] : network->getNodes()) {
        if (!node->getGround()){

            // 1. List all connecting angles with angle and inflow
            std::vector<RadialPosition<T>> channelOrder;
            for (auto& [channelId, channel] : network->getChannels()) {
                bool inflow;
                arch::Node<T>* nodeA = network->getNode(channel->getNodeA()).get();
                arch::Node<T>* nodeB = network->getNode(channel->getNodeB()).get();
                T dx = ( nodeId == channel->getNodeA() ) ? nodeB->getPosition()[0]-nodeA->getPosition()[0] : nodeA->getPosition()[0]-nodeB->getPosition()[0];
                T dy = ( nodeId == channel->getNodeA() ) ? nodeB->getPosition()[1]-nodeA->getPosition()[1] : nodeA->getPosition()[1]-nodeB->getPosition()[1];
                T angle = atan2(dy,dx);
                angle = std::fmod(atan2(dy,dx)+2*M_PI,2*M_PI);
                if ((channel->getFlowRate() > 0.0 && channel->getNodeB() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeA() == nodeId)) {
                    // Channel is inflow channel
                    inflow = true;
                } else if ((channel->getFlowRate() > 0.0 && channel->getNodeA() == nodeId) || (channel->getFlowRate() < 0.0 && channel->getNodeB() == nodeId)) {
                    // Channel is outflow channel
                    inflow = false;
                } else {
                    // Channel has no flow rate: Do nothing.
                    continue;
                }
                RadialPosition<T> newPosition = {angle, channel->getId(), inflow};
                channelOrder.push_back(newPosition);
            }

            // 2. Order connected channels according to radial angle
            std::sort(channelOrder.begin(), channelOrder.end(), [](auto& a, auto& b) {
                return a.radialAngle < b.radialAngle;  // ascending order
            });

            // 3. Group connected channels into groups of
            //     - Concatenated inflows
            //     - Concatenated outflows
            while (!channelOrder.empty()) {
                std::vector<RadialPosition<T>> concatenatedFlow;
                concatenatedFlow.emplace_back( std::move(channelOrder.front()) );
                channelOrder.erase(channelOrder.begin());
                int n = 0;(relative, 0.0-1.0)
                for (auto& flow : channelOrder) {
                    if (flow.inFlow == concatenatedFlow.back().inFlow) {
                        concatenatedFlow.emplace_back( std::move(channelOrder.front()) );
                        n++;
                    } else {
                        break;
                    }
                }

                channelOrder.erase(channelOrder.begin(), channelOrder.begin() + n);
                concatenatedFlows.push_back(concatenatedFlow);
            }
            if (concatenatedFlows.front().front().inFlow == concatenatedFlows.back().back().inFlow) {
                for (auto& flow : concatenatedFlows.front()) {
                    concatenatedFlows.back().emplace_back( std::move(flow) );
                }
                concatenatedFlows.erase(concatenatedFlows.begin());
            }

            // 4. Calculate the flow connections between inflow/outflow channels
            // and generate outflowDistribution.
            if (concatenatedFlows.size() == 2) {
                int in = (concatenatedFlows[0][0].inFlow) ? 0 : 1;
                int out = (concatenatedFlows[0][0].inFlow) ? 1 : 0;
                T flowRateIn;
                T flowRateOut;
                for (auto& channel : concatenatedFlows[in]) {
                    flowRateIn += network->getChannel(channel.channelId)->getFlowRate();
                }
                for (auto& channel : concatenatedFlows[out]) {
                    flowRateOut += network->getChannel(channel.channelId)->getFlowRate();
                }

                std::vector<T> inflowCuts = {0.0};
                std::vector<int> channelInIDs;
                // Calculate inflow separations
                for (auto& channelIn = concatenatedFlows[in].rbegin(); channelIn != concatenatedFlows[in].rend(); ++channelIn) {
                    T newCut = inflowCuts.back() + network->getChannel(channelIn.channelId)->getFlowRate()/flowRateIn;
                    inflowCuts.push_back(std::min(newCut, 1.0));
                    channelInIDs.push_back(channelIn.channelId);
                }

                std::vector<T> outflowCuts = {0.0};
                std::vector<int> channelOutIDs;
                for (auto& channelOut : concatenatedFlows[out]) {
                    T newCut = outflowCuts.back() + network->getChannel(channelOut.channelId)->getFlowRate()/flowRateOut;
                    outflowCuts.push_back(std::min(newCut, 1.0));
                    channelOutIDs.push_back(channelOut.channelId);
                }

                int n_in = 0;
                int n_out = 0;
                T start = 0.0;
                T end = 0.0;
                for (auto& channelOutId : channelOutIDs) {
                    for (auto& channelInId : channelInIDs) {
                        FlowSection<T> inFlowSection;
                        inFlowSection.channelId = channelInId;
                        if (inflowCut[n_in + 1] < outflowCut[n_out + 1]) {
                            T flowRate = (1.0 - start) * network->getChannel(channelInId)->getFlowRate();
                            inFlowSection = FlowSection<T> {channelInId, start, 1.0, flowRate};
                            n_in++;
                            start = 0.0;
                        } else {
                            T flowRate = (outflowCuts[n_out + 1] - std::max(inflowCuts[n_in], outflowCuts[n_out]))*flowRateIn;
                            end = start + flowRate / network->getChannel(channelInId)->getFlowRate();
                            inFlowSection = FlowSection<T> {channelInId, start, end, flowRate};
                            n_out++;
                            start = end;
                            break;
                        }
                        if (outflowDistributions.count(channelOutId)) {
                            outflowDistributions.at(channelOutId).push_back(inFlowSection);
                        } else {
                            std::vector<FlowSection<T>> newFlowSectionVector = {inFlowSection};
                            outflowDistributions.try_emplace(channelOutId, newFlowSectionVector);
                        }
                    }                    
                }

            } else if (concatenatedFlows.size() == 4){
                // Assume case with 2 pairs of opposing channels
                for (auto& concatenatedFlow = concatenatedFlows.begin(); concatenatedFlow != concatenatedFlows.end(); ++concatenatedFlow) {
                    if (concatenatedFlow->at(0).inflow) {
                        // Close the loop of the concatenatedFlows vector
                        auto& clockWise = concatenatedFlow + ((concatenatedFlow == concatenatedFlows.end()) ? concatenatedFlows.begin() : 1);
                        int channelInId = concatenatedFlow->at(0).channelId;
                        int channelOutId = clockWise->at(0).channelId;
                        FlowSection<T> inFlowSection {channelInId, 0.0, 0.5, 0.5*network->getChannel(channelInId)->getFlowRate()};
                        std::vector<FlowSection<T>> newFlowSectionVector = {inFlowSection};
                        outflowDistributions.try_emplace(channelOutId, newFlowSectionVector);
                    }
                }
                for (auto& concatenatedFlow = concatenatedFlows.begin(); concatenatedFlow != concatenatedFlows.end(); ++concatenatedFlow) {
                    if (concatenatedFlow->at(0).inflow) {
                        // Close the loop of the concatenatedFlows vector
                        auto& antiClockWise = concatenatedFlow + ((concatenatedFlow == concatenatedFlows.begin()) ? concatenatedFlows.end() : -1);
                        int channelInId = concatenatedFlow->at(0).channelId;
                        int channelOutId = antiClockWise->at(0).channelId;
                        FlowSection<T> inFlowSection {channelInId, 0.5, 1.0, 0.5*network->getChannel(channelInId)->getFlowRate()};
                        outflowDistributions.at(channelOutId).push_back(inFlowSection);
                    }
                }
            } else {
                throw std::invalid_argument("Too many channels at node " << nodeId << " for DiffusionMixingModel.");
            }
        }
    }
}

template<typename T>
std::pair<std::function<T(T)>,std::vector<T>>DiffusionMixingModel<T>::getAnalyticalSolutionConstant(T channelLength, T channelWidth, int resolution, T pecletNr, const std::vector<FlowSectionInput<T>>& parameters) { // From Channel Start to Channel End for constant input!!
    // Get the concentration distribution at the end of the channel along its width w
    // resolution defines the number of times the fourier series is solved for (?)
    /*
    * parameter would look something like this
    *    std::vector<channelStartSectionsConstant> parameter = {
    *    {1.0, 5.0, 1.0}, 
    *    {3.0, 7.0, 2.0e-3} 
    *   };  
    */
    T a_0 = 0.0;
    std::vector<T> segmentedResult;
    // T a_n = 0.0;


    for (const auto& parameter : parameters) { // iterates through all channels that flow into the current channel
            a_0 += parameter.concentration * (parameter.endWidth - parameter.startWidth);
        }

    auto f = [a_0, channelLength, channelWidth, resolution, pecletNr, parameters, &segmentedResult](T w) { // This returns C(w, l_1)
        T f_sum = 0.0;

        for (int n = 1; n < resolution; n++) {
            for (const auto& parameter : parameters) {
                T a_n = 2 * parameter.concentration * n * (sin(n * M_PI * parameter.endWidth) - sin(n * M_PI * parameter.startWidth));
                f_sum += a_n * std::cos(0.5 * n * M_PI * w) * std::exp(-pow(n, 2) * pow(M_PI, 2) * (1 / pecletNr) * channelLength);
                segmentedResult.push_back(a_n * std::exp(-pow(n, 2) * pow(M_PI, 2) * (1 / pecletNr) * channelLength)); // * std::cos(0.5 * n * M_PI * w) // store that in the other struct as well?
            }
        }
        return 0.5 * a_0 + f_sum;
    };
    return {f, segmentedResult};
}

template<typename T> // TODO maybe combine this function with the function above
std::pair<std::function<T(T)>,std::vector<T>> DiffusionMixingModel<T>::getAnalyticalSolutionFunction(T channelLength, T channelWidth, int resolution, T pecletNr, const std::vector<FlowSectionInput<T>>& parameters, std::function<T(T)> fConstant) { // From Channel Start to Channel End for complex input
    T a_0 = 0.0;
    std::vector<T> segmentedResult;
    // T a_n = 0.0;

    // for (const auto& parameter : parameters) { // iterates through all channels that flow into the current channel
    //         a_0 += parameter.concentration * (parameter.endWidth - parameter.startWidth); // TODO adapt this for 
    //     }
    T stretchFactor = parameters.stretchFactor;
    T startWidthIfFunctionWasSplit = parameters.startWidthIfFunctionWasSplit;

    auto f = [a_0, channelLength, channelWidth, resolution, pecletNr, parameters, stretchFactor, startWidthIfFunctionWasSplit, &segmentedResult, fConstant](T w) { // This returns C(w, l_1)
        T f_sum = 0.0;
        T a_n = 0.0;
        for (int n = 1; n < resolution; n++) {    
            for (const auto& parameter : parameters) {
                for (int i = 0; i < len(parameter.segmentedResult); i++) {
                    T a_0 = 2 / M_PI * ((parameter.endWidth - parameter.startWidth) + parameter.segmentedResult[i] * n * M_PI * (sin(n * M_PI * parameter.endWidth) - sin(n * M_PI * parameter.startWidth)));
                    // T a_0 = 2 / M_PI * parameter.segmentedResult[i] * n * M_PI * (sin(n * M_PI * parameter.endWidth) - sin(n * M_PI * parameter.startWidth));

                    a_n += (2 / M_PI) * 0.5 *
                    ((n * M_PI * (1/stretchFactor + 1)) * 
                    (std::sin(-startWidthIfFunctionWasSplit * 1/stretchFactor + parameters.endWidth * (n * M_PI * (1/stretchFactor + 1))) 
                    - std::sin(-startWidthIfFunctionWasSplit * 1/stretchFactor + parameters.startWidth * (n * M_PI * (1/stretchFactor + 1))) 
                    - (n * M_PI * (1/stretchFactor + 1)) * 
                    (std::sin(-startWidthIfFunctionWasSplit * 1/stretchFactor + parameters.endWidth * (n * M_PI * (1/stretchFactor + 1))) 
                    - std::sin(-startWidthIfFunctionWasSplit * 1/stretchFactor + parameters.startWidth * (n * M_PI * (1/stretchFactor + 1)))))) 
                    * parameter.segmentedResult[i];
                }
                // T a_n = 2 * parameter.concentration * n * (sin(n * M_PI * parameter.endWidth) - sin(n * M_PI * parameter.startWidth));
                f_sum += a_n * std::cos(0.5 * n * M_PI * w) * std::exp(-pow(n, 2) * pow(M_PI, 2) * (1 / pecletNr) * channelLength);
                segmentedResult.push_back(a_n * std::exp(-pow(n, 2) * pow(M_PI, 2) * (1 / pecletNr) * channelLength));
            }
        }
        return 0.5 * a_0 + f_sum + fConstant;
    };
    return {f, segmentedResult};
}

template<typename T>
std::pair<std::function<T(T)>,std::vector<T>> DiffusionMixingModel<T>::getAnalyticalSolutionTotal(T channelLength, T currChannelFlowRate, T channelWidth, int resolution, T pecletNr, const std::vector<FlowSection<T>>& flowSections) { // From Channel Start to Channel End for complex input

    T prevEndWidth = 0.0;

    std::vector<FlowSectionInput<T>> constantFlowSections;
    std::vector<FlowSectionInput<T>> functionFlowSections;

    for (const auto& flowSection : flowSections) {
        T startWidth = prevEndWidth;
        T endWidth = startWidth + flowSection.flowRate / currChannelFlowRate;
        prevEndWidth = endWidth;
        T stretchFactor = flowSection.flowRate / currChannelFlowRate;
        T startWidthIfFunctionWasSplit = flowSection.startWidth;

        if (flowSection.isConstant == true) {
            constantFlowSections.push_back({startWidth, endWidth, stretchFactor, startWidthIfFunctionWasSplit, flowSection.concentrationAtChannelEndFunction, flowSection.segmentedResult});
        } else {
            functionFlowSections.push_back({startWidth, endWidth, stretchFactor, startWidthIfFunctionWasSplit, flowSection.concentrationAtChannelEndFunction, flowSection.segmentedResult});
        }
    }

    auto [fConstant, segmentedResultConstant] = getAnalyticalSolutionConstant(channelLength, channelWidth, resolution, pecletNr, constantFlowSections);
    auto [fFunction, segmentedResultFunction] = getAnalyticalSolutionFunction(channelLength, channelWidth, resolution, pecletNr, functionFlowSections, fConstant);

    segmentedResultFunction.insert(segmentedResultFunction.end(), segmentedResultConstant.begin(), segmentedResultConstant.end());

    return {fFunction, segmentedResultFunction};
}



template<typename T>
void DiffusionMixingModel<T>::printTopology() {
    int iteration = 0;
    for (auto& flows : concatenatedFlows) {
        std::cout << "Group " << iteration << std::endl;
        for (auto& flow : flows) {
            std::cout << "Channel " << flow.channelId << " has radial angle = " << flow.radialAngle
                << " and is ";
            if (!flow.inFlow) {
                std::cout << " an outflow.\n";
            } else {
                std::cout << " an inflow.\n";
            }
        }
        std::cout << "\n";
        iteration++;
    }
}

template<typename T>
void DiffusionMixingModel<T>::updateNodeInflow() {
    // TODO
}

template<typename T>
void DiffusionMixingModel<T>::generateInflows() {
    // TODO
}

template<typename T>
void DiffusionMixingModel<T>::generateNodeOutflow() {
    // TODO
}

template<typename T>
void DiffusionMixingModel<T>::updateChannelInflow() {
    // TODO
}

}   /// namespace sim