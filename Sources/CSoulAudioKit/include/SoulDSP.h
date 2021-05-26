// Copyright AudioKit. All Rights Reserved.

#ifndef SoulDSP_hpp
#define SoulDSP_hpp

#import "DSPBase.h"

template<typename SoulPatchType>
class SoulDSP : public DSPBase {

public:
    SoulPatchType patch;
    
    using MIDIMessage = typename SoulPatchType::MIDIMessage;
    using ParameterProperties = typename SoulPatchType::ParameterProperties;
    using ParameterList = typename SoulPatchType::ParameterList;
    
    std::vector<MIDIMessage> midiMessages;
    ParameterList params;
    
    SoulDSP() {
        // Reserve space for MIDI messages so we don't have to allocate.
        midiMessages.reserve(1024);

        params = patch.createParameterList();
    }
    
    void setParameter(AUParameterAddress address, float value, bool immediate) override {
        if(address < params.size()) {
            params[address].setValue(value);
        }
    }
    
    void init(int channelCount, double sampleRate) override {
        // I'm not sure what sessionID is for.
        patch.init(sampleRate, /*sessionID*/ 0);
        
        // init will clear the properties, so set them back to their value
        for(auto& param : params) {
            param.setValue(param.getValue());
        }
    }
    
    void reset() override {
        patch.reset();
    }
    
    void handleMIDIEvent(AUMIDIEvent const& midiEvent) override {
        MIDIMessage message;
        message.frameIndex = 0;
        message.byte0 = midiEvent.data[0];
        message.byte1 = midiEvent.data[1];
        message.byte2 = midiEvent.data[2];
        midiMessages.push_back(message);
    }

    void startRamp(const AUParameterEvent &event) override {
        auto address = event.parameterAddress;
        if(address < params.size()) {
            params[address].setValue(event.value);
        }
    }
    
    // Need to override this since it's pure virtual.
    void process(FrameRange range) override {
        
        // Zero output channels.
        for(int channel=0; channel<channelCount; ++channel) {
            float* outputChannel = ((float *)outputBufferList->mBuffers[channel].mData) + range.start;
            std::fill(outputChannel, outputChannel+range.count, 0.0f);
        }
        
        typename SoulPatchType::template RenderContext<float> context;
        
        context.numFrames = range.count;
        context.inputChannels[0] = ((const float *)inputBufferLists[0]->mBuffers[0].mData) + range.start;
        context.outputChannels[0] = ((float *)outputBufferList->mBuffers[0].mData) + range.start;
        context.incomingMIDI.messages = midiMessages.data();
        context.incomingMIDI.numMessages = (uint32_t) midiMessages.size();
        
        patch.render(context);
        
        midiMessages.clear();
        
    }
    
};

#endif /* SoulDSP_hpp */
