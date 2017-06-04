/*
  ==============================================================================

    TreePlayer.h
    Created: 10 May 2017 7:43:27pm
    Author:  blist

  ==============================================================================
*/

#ifndef TREEPLAYER_H_INCLUDED
#define TREEPLAYER_H_INCLUDED

#include "JuceDemoHeader.h"
#include "ValueTreeItem.h"

class TreePlayer
{
public:
    TreePlayer()
    {

    }

    ~TreePlayer()
    {

    }

	void load(ValueTree t)
	{
		tree = t;
	}

    void setNoteDurations() {
		for (int i = 0; i < tree.getNumChildren(); ++i)
			addSubItem(new ValueTreeItem(tree.getChild(i), undoManager));
		}
		/*LeafNoteTreeNode node = new LeafNoteTreeNode("1");
		while (nodeEnum.hasMoreElements()) {

			Object nextElement = nodeEnum.nextElement();
			if (nextElement instanceof LeafNoteTreeNode) {
				node = (LeafNoteTreeNode) nextElement;
				node.setRhythmValue(node.getNodeNoteDuration());

			}

		}*/

	}
	
	//float getNodeNoteDuration() {
	//	return getNodeNoteDuration(this);
	//}
 //   
 //   float getNodeNoteDuration(LeafNoteTreeNode node) {
	//	LeafNoteTreeNode parent = (LeafNoteTreeNode) node.getParent();
	//	if (parent != null) {
	//		return getNodeNoteDuration(parent) / parent.getChildCount();
	//	} else
	//		return 4.00;
	//}
	//
	//Iterator<LeafNoteTreeNode> LeftToRightLeafTraversal() {
	//	List<LeafNoteTreeNode> leftToRightLeafList = new ArrayList<LeafNoteTreeNode>();
	//	return LeftToRightLeafTraversal(leftToRightLeafList, this).iterator();
	//}

	//List<LeafNoteTreeNode> LeftToRightLeafTraversal(
	//		List<LeafNoteTreeNode> leftToRightLeafList, LeafNoteTreeNode node) {

	//	if (node.isLeaf()) {
	//		leftToRightLeafList.add(node);
	//	} else {
	//		Enumeration childEnum = node.children();
	//		while (childEnum.hasMoreElements()) {
	//			LeftToRightLeafTraversal(leftToRightLeafList,
	//					(LeafNoteTreeNode) childEnum.nextElement());
	//		}

	//	}
	//	return leftToRightLeafList;

	//}
	//
	//void buildLeafNotes(){
	//	this.notesInTree.clear();
	//	setNoteDurations();

	//	Iterator<LeafNoteTreeNode> iter = LeftToRightLeafTraversal();
	//	
	//	Object userObject;
	//	while (iter.hasNext()) {
	//		LeafNoteTreeNode nextNode = iter.next();
	//		userObject = nextNode.getUserObject();
	//		Note rest;
	//		if (userObject instanceof String) {
	//			String str = (String) userObject;
	//			if(str.equalsIgnoreCase("R")){
	//				rest = new Note();
	//				rest.setRest(true);
	//				this.notesInTree.add(rest);
	//			}
	//			// else the note is tied to the next note.
	//			else if (str.contains("t")){
	//				nextNode.setTie(true);
	//				int tieNoteValue = nextNode.getPitchValue();
	//				System.out.println("tie Note value in tree: " + tieNoteValue);
	//				double tieNoteDecimalDuration = nextNode.getRhythmValue();
	//				Object userObject2;
	//				if(iter.hasNext()){
	//				LeafNoteTreeNode nextNextNode = iter.next();
	//				
	//				userObject2 = nextNextNode.getUserObject();
	//				String str2 = (String) userObject2;
	//				
	//				boolean done = false;
	//				while(nextNode.hasSamePitch(nextNextNode) && !done){
	//					tieNoteDecimalDuration += nextNextNode.getRhythmValue();
	//					if(str2.contains("t") && iter.hasNext()){
	//						nextNextNode = iter.next();
	//					}
	//					else{
	//						 done = true;
	//					}
	//					
	//					userObject2 = nextNextNode.getUserObject();
	//					str2 = (String) userObject2;
	//				}
	//				this.notesInTree.add(new Note(tieNoteValue, tieNoteDecimalDuration));
	//				}
	//			}
	//			else{
	//				nextNode.setTie(false);
	//				this.notesInTree.add(nextNode.getNote());
	//			}
	//			
	//		}

	//	}
	//}
	//
	//void playSubPhrase() {
	//	Play.midi(this.getSubPhrase());
	//}
	//
	//Phrase getSubPhrase() {
	//	this.buildSubPhrase();
	//	return subPhrase;
	//}
	//
	//void buildSubPhrase() {
	//	// clear subPart
	//	subPhrase = new Phrase();
	//	// add tempo info
	//	//subPart.add("T[Adagio]");
	//	
	//	this.buildLeafNotes();
	//	Iterator<Note> notes = this.notesInTree.iterator();
	//	while(notes.hasNext()){
	//		Note note = notes.next();
	//		this.subPhrase.add(note);
	//	}

	//}

 //   Note getNote() {
	//	return this.note;
	//}

private:
	TreeView tree;
};

#endif  // TREEPLAYER_H_INCLUDED
