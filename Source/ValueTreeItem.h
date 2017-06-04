/*
  ==============================================================================

    ValueTreeItem.h
    Created: 10 May 2017 9:04:40pm
    Author:  blist

  ==============================================================================
*/
#include "JuceDemoHeader.h"

#ifndef VALUETREEITEM_H_INCLUDED
#define VALUETREEITEM_H_INCLUDED

//==============================================================================
class ValueTreeItem  : public TreeViewItem,
private ValueTree::Listener
{
public:
    ValueTreeItem (const ValueTree& v, UndoManager& um)
    : tree (v), undoManager (um)
    {
        tree.addListener (this);
    }
    
    String getUniqueName() const override
    {
        return tree["name"].toString();
    }
    
    bool mightContainSubItems() override
    {
        return tree.getNumChildren() > 0;
    }
    
    void paintItem (Graphics& g, int width, int height) override
    {
        g.setColour (Colours::black);
        g.setFont (15.0f);
        
        g.drawText (tree["name"].toString(),
                    4, 0, width - 4, height,
                    Justification::centredLeft, true);
    }
    
    void itemOpennessChanged (bool isNowOpen) override
    {
        if (isNowOpen && getNumSubItems() == 0)
            refreshSubItems();
        else
            clearSubItems();
    }
    
    var getDragSourceDescription() override
    {
        return "Drag Demo";
    }
    
    bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& dragSourceDetails) override
    {
        return dragSourceDetails.description == "Drag Demo";
    }
    
    void itemDropped (const DragAndDropTarget::SourceDetails&, int insertIndex) override
    {
        OwnedArray<ValueTree> selectedTrees;
        getSelectedTreeViewItems (*getOwnerView(), selectedTrees);
        
        moveItems (*getOwnerView(), selectedTrees, tree, insertIndex, undoManager);
    }
    
    static void moveItems (TreeView& treeView, const OwnedArray<ValueTree>& items,
                           ValueTree newParent, int insertIndex, UndoManager& undoManager)
    {
        if (items.size() > 0)
        {
            ScopedPointer<XmlElement> oldOpenness (treeView.getOpennessState (false));
            
            for (int i = items.size(); --i >= 0;)
            {
                ValueTree& v = *items.getUnchecked(i);
                
                if (v.getParent().isValid() && newParent != v && ! newParent.isAChildOf (v))
                {
                    if (v.getParent() == newParent && newParent.indexOf(v) < insertIndex)
                        --insertIndex;
                    
                    v.getParent().removeChild (v, &undoManager);
                    newParent.addChild (v, insertIndex, &undoManager);
                }
            }
            
            if (oldOpenness != nullptr)
                treeView.restoreOpennessState (*oldOpenness, false);
        }
    }
    
    static void getSelectedTreeViewItems (TreeView& treeView, OwnedArray<ValueTree>& items)
    {
        const int numSelected = treeView.getNumSelectedItems();
        
        for (int i = 0; i < numSelected; ++i)
            if (const ValueTreeItem* vti = dynamic_cast<ValueTreeItem*> (treeView.getSelectedItem (i)))
                items.add (new ValueTree (vti->tree));
    }
    
private:
    ValueTree tree;
    UndoManager& undoManager;
    
    void refreshSubItems()
    {
        clearSubItems();
        
        for (int i = 0; i < tree.getNumChildren(); ++i)
            addSubItem (new ValueTreeItem (tree.getChild (i), undoManager));
    }
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override
    {
        repaintItem();
    }
    
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override         { treeChildrenChanged (parentTree); }
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override  { treeChildrenChanged (parentTree); }
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override    { treeChildrenChanged (parentTree); }
    void valueTreeParentChanged (ValueTree&) override {}
    
    void treeChildrenChanged (const ValueTree& parentTree)
    {
        if (parentTree == tree)
        {
            refreshSubItems();
            treeHasChanged();
            setOpen (true);
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeItem)
};



#endif  // VALUETREEITEM_H_INCLUDED
