/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#include "JuceDemoHeader.h"
#include "MidiPlayer.h"


//==============================================================================
class AudioSynthesiserDemo  : public Component,
                              private Button::Listener
{
public:
    AudioSynthesiserDemo()
        : deviceManager (MainAppWindow::getSharedAudioDeviceManager()),
          synthAudioSource (keyboardState),
          keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
        addAndMakeVisible (keyboardComponent);

        addAndMakeVisible (sineButton);
        sineButton.setButtonText ("Use sine wave");
        sineButton.setRadioGroupId (321);
        sineButton.addListener (this);
        sineButton.setToggleState (true, dontSendNotification);

        addAndMakeVisible (sampledButton);
        sampledButton.setButtonText ("Use sampled sound");
        sampledButton.setRadioGroupId (321);
        sampledButton.addListener (this);

		addAndMakeVisible(chordLabel);
		chordLabel.setEditable(true);
		chordLabel.setText("M7", dontSendNotification);

        //addAndMakeVisible (liveAudioDisplayComp);

        //deviceManager.addAudioCallback (&liveAudioDisplayComp);

        audioSourcePlayer.setSource (&synthAudioSource);

        deviceManager.addAudioCallback (&audioSourcePlayer);
        deviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));

        setOpaque (true);
        setSize (640, 480);
    }

    ~AudioSynthesiserDemo()
    {
        audioSourcePlayer.setSource (nullptr);
        deviceManager.removeMidiInputCallback (String(), &(synthAudioSource.midiCollector));
        deviceManager.removeAudioCallback (&audioSourcePlayer);
        //deviceManager.removeAudioCallback (&liveAudioDisplayComp);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        fillStandardDemoBackground (g);
    }

    void resized() override
    {
        keyboardComponent.setBounds (8, 96, getWidth() - 16, 64);
        sineButton.setBounds (16, 176, 150, 24);
        sampledButton.setBounds (16, 200, 150, 24);
		chordLabel.setBounds(16, 226, 150, 24);
        //liveAudioDisplayComp.setBounds (8, 8, getWidth() - 16, 64);
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
    //LiveScrollingAudioDisplay liveAudioDisplayComp;
    MidiPLayer midiPlayer;

    //==============================================================================
    void buttonClicked (Button* buttonThatWasClicked) override
    {
		if (buttonThatWasClicked == &sineButton) {
			Bach::ChordReader cr;
			keyboardState.allNotesOff(0);
			for (auto n : cr.midiChord(chordLabel.getText(), 57)) {
                midiPlayer.addNote(n, 120, 0, 1);
			}

		}
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSynthesiserDemo)
};


// This static object will register this demo type in a global list of demos..
static JuceDemoType<AudioSynthesiserDemo> demo ("31 Audio: Synthesisers");
