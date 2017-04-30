/*
  ==============================================================================

    MidiPlayer.h
    Created: 30 Apr 2017 10:17:02am
    Author:  Andrew Liston

  ==============================================================================
*/

#ifndef MIDIPLAYER_H_INCLUDED
#define MIDIPLAYER_H_INCLUDED


#include "JuceDemoHeader.h"

//==============================================================================
/** Our demo synth sound is just a basic sine wave.. */
struct SineWaveSound : public SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote (int /*midiNoteNumber*/) override        { return true; }
    bool appliesToChannel (int /*midiChannel*/) override        { return true; }
};


//==============================================================================
/** Our demo synth voice just plays a sine wave.. */
struct SineWaveVoice  : public SynthesiserVoice
{
    SineWaveVoice()   : currentAngle (0), angleDelta (0), level (0), tailOff (0)
    {
    }

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.

            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..

            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved (int /*newValue*/) override
    {
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float) (std::sin (currentAngle) * level * tailOff);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99;

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();

                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float) (std::sin (currentAngle) * level);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle, angleDelta, level, tailOff;
};

//==============================================================================
// This is an audio source that streams the output of our demo synth.
struct SynthAudioSource  : public AudioSource
{
    SynthAudioSource (MidiKeyboardState& keyState)  : keyboardState (keyState)
    {
        // Add some voices to our synth, to play the sounds..
        for (int i = 4; --i >= 0;)
        {
            synth.addVoice (new SineWaveVoice());   // These voices will play our custom sine-wave sounds..
        }

        // ..and add a sound for them to play...
        setUsingSineWaveSound();
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
        synth.addSound (new SineWaveSound());
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        midiCollector.reset (sampleRate);

        synth.setCurrentPlaybackSampleRate (sampleRate);
    }

    void releaseResources() override
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // the synth always adds its output to the audio buffer, so we have to clear it
        // first..
        bufferToFill.clearActiveBufferRegion();

        // fill a midi buffer with incoming messages from the midi input.
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);

        // pass these messages to the keyboard state so that it can update the component
        // to show on-screen which keys are being pressed on the physical midi keyboard.
        // This call will also add midi messages to the buffer which were generated by
        // the mouse-clicking on the on-screen keyboard.
        keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);

        // and now get the synth to process the midi events and generate its output.
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }

    //==============================================================================
    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    MidiMessageCollector midiCollector;

    // this represents the state of which keys on our on-screen keyboard are held
    // down. When the mouse is clicked on the keyboard component, this object also
    // generates midi messages for this, which we can pass on to our synth.
    MidiKeyboardState& keyboardState;

    // the synth itself!
    Synthesiser synth;
};

//==============================================================================
class MidiPLayer : private Timer
{
public:
    MidiPLayer()
        : deviceManager (MainAppWindow::getSharedAudioDeviceManager()),
          synthAudioSource (keyboardState),
          keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
          midiChannel (10), startTime (Time::getMillisecondCounterHiRes() * 0.001),
          sampleRate (44100.0),
          previousSampleNumber (0),
          tempo(100)
    {

        audioSourcePlayer.setSource (&synthAudioSource);
        deviceManager.addAudioCallback (&audioSourcePlayer);
        deviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));
        startTimer (1);

    }

    ~MidiPLayer()
    {
        audioSourcePlayer.setSource (nullptr);
        deviceManager.removeMidiInputCallback (String(), &(synthAudioSource.midiCollector));
        deviceManager.removeAudioCallback (&audioSourcePlayer);
    }
    
    void setTempo(double t) {
        if (t > 0)
        tempo = t;
    }
    
    void addNote (int midiNoteNumber, int velocity,
                   double start, double duration)
    {
        if (midiNoteNumber >= 0)
        {
            MidiMessage message = MidiMessage::noteOn (1, midiNoteNumber, (uint8) velocity);

            message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001 - startTime + start);

            addMessageToBuffer(message);
            
            
            MidiMessage messageOff (MidiMessage::noteOff (message.getChannel(), message.getNoteNumber()));
            messageOff.setTimeStamp (message.getTimeStamp() + duration);
            addMessageToBuffer(messageOff);
        }
    }
    
    
    //==============================================================================
private:
    void timerCallback() override
    {
        double currentTime = Time::getMillisecondCounterHiRes() * 0.001 - startTime;
        const int currentSampleNumber =  (int) (currentTime * sampleRate); // [4]
        
        MidiBuffer::Iterator iterator (midiBuffer);
        MidiMessage message;
        int sampleNumber;
        
        while (iterator.getNextEvent (message, sampleNumber)) // [5]
        {
            if (sampleNumber > currentSampleNumber)           // [6]
                break;
            
            message.setTimeStamp (sampleNumber / sampleRate); // [7]
            addMessageToList (message);
        }
        
        midiBuffer.clear (previousSampleNumber, currentSampleNumber - previousSampleNumber); // [8]
        previousSampleNumber = currentSampleNumber;                                          // [9]
    }
    
    void addMessageToList (const MidiMessage& message)
    {
        synthAudioSource.midiCollector.addMessageToQueue (message);
        
        const double time = message.getTimeStamp();
        const int hours   = ((int) (time / 3600.0)) % 24;
        const int minutes = ((int) (time / 60.0)) % 60;
        const int seconds = ((int) time) % 60;
        const int millis  = ((int) (time * 1000.0)) % 1000;
        const String timecode (String::formatted ("%02d:%02d:%02d.%03d",
                                                  hours,
                                                  minutes,
                                                  seconds,
                                                  millis));
        logMessage (timecode + "  -  " + getMidiMessageDescription (message));
    }
    
    void logMessage (const String& m)
    {
        // prints a message to the console
        DBG(m);
    }
    
    void addMessageToBuffer (const MidiMessage& message)
    {
        const double timestamp = message.getTimeStamp();
        const int sampleNumber =  (int) (timestamp * sampleRate);
        midiBuffer.addEvent (message, sampleNumber);
    }
    
    static String getMidiMessageDescription (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "  + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change " + String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel " + String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";
        
        if (m.isController())
        {
            String name (MidiMessage::getControllerName (m.getControllerNumber()));
            
            if (name.isEmpty())
                name = "[" + String (m.getControllerNumber()) + "]";
            
            return "Controller " + name + ": " + String (m.getControllerValue());
        }
        
        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }
    

    

private:
    AudioDeviceManager& deviceManager;
    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;
    MidiKeyboardComponent keyboardComponent;
    ToggleButton sineButton;
    ToggleButton sampledButton;
	Label chordLabel;
    int midiChannel;
    double startTime;
    
    MidiBuffer midiBuffer;    // [1]
    double sampleRate;        // [2]
    int previousSampleNumber; // [3]
    double tempo;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPLayer)
};







#endif  // MIDIPLAYER_H_INCLUDED
