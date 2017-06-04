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

        addAndMakeVisible (playButton);
        playButton.setButtonText ("Play");
		playButton.addListener (this);

		addAndMakeVisible(chordLabel);
		chordLabel.setEditable(true);
		chordLabel.setText("CM7", dontSendNotification);

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
		playButton.setBounds (16, 176, 150, 24);
		chordLabel.setBounds(16, 226, 150, 24);
        //liveAudioDisplayComp.setBounds (8, 8, getWidth() - 16, 64);
    }

private:
    AudioDeviceManager& deviceManager;
    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;
    MidiKeyboardComponent keyboardComponent;
    TextButton playButton;
	Label chordLabel;
    //LiveScrollingAudioDisplay liveAudioDisplayComp;
    MidiPLayer midiPlayer;
	int i = 0;

    //==============================================================================
    void buttonClicked (Button* buttonThatWasClicked) override
    {
		if (buttonThatWasClicked == &playButton) {
			keyboardState.allNotesOff(0);
			Bach::MidiUtils mUtils;
			//Bach::Chord c(chordLabel.getText());
			
			Bach::Chord c("C" + mUtils.SUPPORTED_CHORD_TYPES.getUnchecked(i));
			playButton.setButtonText("C" + mUtils.SUPPORTED_CHORD_TYPES.getUnchecked(i));
			for (auto n : c.getMidiNoteNumbers()) {
				midiPlayer.addNote(n, 120, 0, 1);
				MidiMessage message = MidiMessage::noteOn(1, n, (uint8)100);
				keyboardState.processNextMidiEvent(message);
			}
			
		}
		i++;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSynthesiserDemo)
};


// This static object will register this demo type in a global list of demos..
static JuceDemoType<AudioSynthesiserDemo> demo ("31 Audio: Synthesisers");
