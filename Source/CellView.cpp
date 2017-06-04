/*
  ==============================================================================

    CellView.cpp
    Created: 24 Apr 2017 8:19:39pm
    Author:  Andrew Liston

  ==============================================================================
*/

#include "JuceDemoHeader.h"
#include "MidiPlayer.h"
#include "ValueTreeItem.h"

//==============================================================================
class ValueTreesDemo   : public Component,
public DragAndDropContainer,
private ButtonListener,
private Timer
{
public:
    ValueTreesDemo()
    : undoButton ("Undo"),
    redoButton ("Redo"),
    addButton ("Add"),
    deleteButton ("Delete")
    {
        addAndMakeVisible (tree);
        
        tree.setDefaultOpenness (true);
        tree.setMultiSelectEnabled (true);
        tree.setRootItem (rootItem = new ValueTreeItem (createRootValueTree(), undoManager));
        tree.setColour (TreeView::backgroundColourId, Colours::white);
        
        addAndMakeVisible (undoButton);
        addAndMakeVisible (redoButton);
        addAndMakeVisible (addButton);
        addAndMakeVisible (deleteButton);
        undoButton.addListener (this);
        redoButton.addListener (this);
        addButton.addListener (this);
        deleteButton.addListener (this);
        
        startTimer (500);
    

    }
    
    ~ValueTreesDemo()
    {
        tree.setRootItem (nullptr);
    }
    
    void paint (Graphics& g) override
    {
        fillStandardDemoBackground (g);
    }
    
    void resized() override
    {
        Rectangle<int> r (getLocalBounds().reduced (8));
        
        Rectangle<int> buttons (r.removeFromBottom (22));
        undoButton.setBounds (buttons.removeFromLeft (100));
        buttons.removeFromLeft (6);
        redoButton.setBounds (buttons.removeFromLeft (100));
        buttons.removeFromLeft (6);
        addButton.setBounds (buttons.removeFromLeft (100));
        buttons.removeFromLeft (6);
        deleteButton.setBounds (buttons.removeFromLeft (100));
        
        r.removeFromBottom (4);
        tree.setBounds (r);
    }
    
    static ValueTree createTree (const String& desc)
    {
        ValueTree t ("Item");
        t.setProperty ("name", desc, nullptr);
        return t;
    }
    
    static ValueTree createRootValueTree()
    {
        ValueTree vt = createTree ("This demo displays a ValueTree as a treeview.");
        vt.addChild (createTree ("You can drag around the nodes to rearrange them"), -1, nullptr);
        vt.addChild (createTree ("..and press 'delete' to delete them"), -1, nullptr);
        vt.addChild (createTree ("Then, you can use the undo/redo buttons to undo these changes"), -1, nullptr);
        
        int n = 1;
        vt.addChild (createRandomTree (n, 0), -1, nullptr);
        
        return vt;
    }
    
    static ValueTree createRandomTree (int& counter, int depth)
    {
        ValueTree t = createTree ("Item " + String (counter++));
        
        if (depth < 3)
            for (int i = 1 + Random::getSystemRandom().nextInt (7); --i >= 0;)
                t.addChild (createRandomTree (counter, depth + 1), -1, nullptr);
        
        return t;
    }
    
    void deleteSelectedItems()
    {
        OwnedArray<ValueTree> selectedItems;
        ValueTreeItem::getSelectedTreeViewItems (tree, selectedItems);
        
        for (int i = selectedItems.size(); --i >= 0;)
        {
            ValueTree& v = *selectedItems.getUnchecked(i);
            
            if (v.getParent().isValid())
                v.getParent().removeChild (v, &undoManager);
        }
    }
    
    void addItems()
    {
        OwnedArray<ValueTree> selectedItems;
        ValueTreeItem::getSelectedTreeViewItems (tree, selectedItems);
        
        for (int i = selectedItems.size(); --i >= 0;)
        {
            ValueTree& v = *selectedItems.getUnchecked(i);
            
            if (v.getParent().isValid())
                v.addChild (createTree ("split item"), v.getParent().indexOf(v), &undoManager);
        }
        
        midiPlayer.addNote(80, 120, 0, 1);
        midiPlayer.addNote(82, 120, 1, .5);
        midiPlayer.addNote(84, 120, 1.5, .5);
        midiPlayer.addNote(86, 120, 2, .25);
        midiPlayer.addNote(87, 120, 2.25, .25);
        midiPlayer.addNote(88, 120, 2.5, .25);
        midiPlayer.addNote(89, 120, 2.75, .25);
        midiPlayer.addNote(88, 120, 3, .333333);
        midiPlayer.addNote(88, 120, 3.333333, .333333);
        midiPlayer.addNote(88, 120, 3.666666, .333333);
        midiPlayer.addNote(80, 120, 4, 1);
    }
    
    
    
    bool keyPressed (const KeyPress& key) override
    {
        if (key == KeyPress::deleteKey)
        {
            deleteSelectedItems();
            return true;
        }
        
        if (key == KeyPress ('z', ModifierKeys::commandModifier, 0))
        {
            undoManager.undo();
            return true;
        }
        
        if (key == KeyPress ('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0))
        {
            undoManager.redo();
            return true;
        }
        
        return Component::keyPressed (key);
    }
    
    void buttonClicked (Button* b) override
    {
        if (b == &undoButton)
            undoManager.undo();
        else if (b == &redoButton)
            undoManager.redo();
        else if (b == &addButton)
            addItems();
        else if (b == &deleteButton)
            deleteSelectedItems();
    }
    
private:
    TreeView tree;
    TextButton undoButton, redoButton, addButton, deleteButton;
    ScopedPointer<ValueTreeItem> rootItem;
    UndoManager undoManager;
    MidiPLayer midiPlayer;
    
    void timerCallback() override
    {
        undoManager.beginNewTransaction();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreesDemo)
};


// This static object will register this demo type in a global list of demos..
static JuceDemoType<ValueTreesDemo> demo ("40 ValueTrees");
