/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]
	// Test parsing
	jassert(chordReader.readChord("-") == Array<String>({ "P1", "m3", "P5" }));
	jassert(chordReader.readChord("m") == Array<String>({ "P1", "m3", "P5" }));
	jassert(chordReader.readChord("mi") == Array<String>({ "P1", "m3", "P5" }));
	jassert(chordReader.readChord("min") == Array<String>({ "P1", "m3", "P5" }));
	jassert(chordReader.readChord("mi7") == Array<String>({ "P1", "m3", "P5", "m7" }));
	jassert(chordReader.readChord("M") == Array<String>({ "P1", "M3", "P5" }));
	jassert(chordReader.readChord("Ma") == Array<String>({ "P1", "M3", "P5" }));
	jassert(chordReader.readChord("M#5") == Array<String>({ "P1", "M3", "A5" }));
	jassert(chordReader.readChord("maj7") == Array<String>({ "P1", "M3", "P5", "M7" }));
	jassert(chordReader.readChord("+") == Array<String>({ "P1", "M3", "A5" }));
	jassert(chordReader.readChord("6") == Array<String>({ "P1", "M3", "P5", "M6" }));
	jassert(chordReader.readChord("#6") == Array<String>({ "P1", "M3", "P5", "A6" }));
	jassert(chordReader.readChord("m6") == Array<String>({ "P1", "m3", "P5", "M6" }));
	jassert(chordReader.readChord("b5") == Array<String>({ "P1", "M3", "d5" }));
	jassert(chordReader.readChord("7b5") == Array<String>({ "P1", "M3", "d5", "m7" }));
	jassert(chordReader.readChord("9") == Array<String>({ "P1", "M3", "P5", "m7", "M9" }));
	jassert(chordReader.readChord("#9") == Array<String>({ "P1", "M3", "P5", "m7", "A9" }));
	jassert(chordReader.readChord("b9") == Array<String>({ "P1", "M3", "P5", "m7", "m9" }));
	jassert(chordReader.readChord("#11") == Array<String>({ "P1", "M3", "P5", "m7", "M9", "A11" }));
	jassert(chordReader.readChord("13") == Array<String>({ "P1", "M3", "P5", "m7", "M9", "P11", "M13" }));
	jassert(chordReader.readChord("13#9b5") == Array<String>({ "P1", "M3", "d5", "m7", "A9", "P11", "M13" }));
	jassert(chordReader.readChord("7sus4") == Array<String>({ "P1", "P4", "P5", "m7" }));
	jassert(chordReader.readChord("maj9") == Array<String>({ "P1", "M3", "P5", "M7", "M9" }));
	jassert(chordReader.readChord("mb6") == Array<String>({ "P1", "m3", "P5", "m6" }));
	jassert(chordReader.readChord("#5#9") == Array<String>({ "P1", "M3", "A5", "m7", "A9" }));
	jassert(chordReader.readChord("m(maj7)") == Array<String>({ "P1", "m3", "P5", "M7" }));
	jassert(chordReader.readChord("m(11b5b9)") == Array<String>({ "P1", "m3", "d5", "m7", "m9", "P11" }));
	jassert(chordReader.readChord("o") == Array<String>({ "P1", "m3", "d5" }));
	jassert(chordReader.readChord("o7") == Array<String>({ "P1", "m3", "d5", "d7" }));
	//jassert(chordReader.readChord("ø") == Array<String>({ "P1", "m3", "d5", "m7" }));
	jassert(chordReader.readChord("m7b5") == Array<String>({ "P1", "m3", "d5", "m7" }));
	jassert(chordReader.readChord("min11") == Array<String>({ "P1", "m3", "P5", "m7", "M9", "P11" }));
	jassert(chordReader.readChord("+M7") == Array<String>({ "P1", "M3", "A5", "M7" }));
	jassert(chordReader.readChord("dom7b5") == Array<String>({ "P1", "M3", "d5", "m7" }));
	jassert(chordReader.readChord("5") == Array<String>({ "P1", "P5" }));
	jassert(chordReader.readChord("add9") == Array<String>({ "P1", "M3", "P5", "M9" }));
	jassert(chordReader.readChord("m13b5#9") == Array<String>({ "P1", "m3", "d5", "m7", "A9", "P11", "M13" }));
	jassert(chordReader.readChord("6/9") == Array<String>({ "P1", "M3", "P5", "M6", "M9" }));
	jassert(chordReader.readChord("M7") == Array<String>({ "P1", "M3", "P5", "M7" }));
	jassert(chordReader.readChord("M9") == Array<String>({ "P1", "M3", "P5", "M7", "M9" }));
	jassert(chordReader.readChord("Ma9") == Array<String>({ "P1", "M3", "P5", "M7", "M9" }));
	jassert(chordReader.readChord("mM7") == Array<String>({ "P1", "m3", "P5", "M7" }));
	String chord = "";
	Array<String> intervals = chordReader.readChord("sus4add9");
	for (auto i : intervals) {
		chord += i + " ";
	}
    addAndMakeVisible (helloWorldLabel = new Label (String(),
                                                    TRANS(chord)));
    helloWorldLabel->setFont (Font (40.00f, Font::bold));
    helloWorldLabel->setJustificationType (Justification::centred);
    helloWorldLabel->setEditable (false, false, false);
    helloWorldLabel->setColour (Label::textColourId, Colours::black);
    helloWorldLabel->setColour (TextEditor::textColourId, Colours::black);
    helloWorldLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (quitButton = new TextButton (String()));
    quitButton->setButtonText (TRANS("Quit"));
    quitButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    helloWorldLabel = nullptr;
    quitButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffc1d0ff));

    g.setColour (Colours::white);
    g.fillPath (internalPath1);
    g.setColour (Colour (0xff6f6f6f));
    g.strokePath (internalPath1, PathStrokeType (5.200f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    helloWorldLabel->setBounds (152, 80, 296, 48);
    quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
    internalPath1.clear();
    internalPath1.startNewSubPath (136.0f, 80.0f);
    internalPath1.quadraticTo (176.0f, 24.0f, 328.0f, 32.0f);
    internalPath1.quadraticTo (472.0f, 40.0f, 472.0f, 104.0f);
    internalPath1.quadraticTo (472.0f, 192.0f, 232.0f, 176.0f);
    internalPath1.lineTo (184.0f, 216.0f);
    internalPath1.lineTo (200.0f, 168.0f);
    internalPath1.quadraticTo (96.0f, 136.0f, 136.0f, 80.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == quitButton)
    {
        //[UserButtonCode_quitButton] -- add your button handler code here..

        JUCEApplication::quit();

        //[/UserButtonCode_quitButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="300">
  <BACKGROUND backgroundColour="ffc1d0ff">
    <PATH pos="0 0 100 100" fill="solid: ffffffff" hasStroke="1" stroke="5.2, mitered, butt"
          strokeColour="solid: ff6f6f6f" nonZeroWinding="1">s 136 80 q 176 24 328 32 q 472 40 472 104 q 472 192 232 176 l 184 216 l 200 168 q 96 136 136 80 x</PATH>
  </BACKGROUND>
  <LABEL name="" id="be4f6f2e5725a063" memberName="helloWorldLabel" virtualName=""
         explicitFocusOrder="0" pos="152 80 296 48" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Hello World!" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="40" bold="1" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="bcf4f7b0888effe5" memberName="quitButton" virtualName=""
              explicitFocusOrder="0" pos="176R 60R 120 32" buttonText="Quit"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
