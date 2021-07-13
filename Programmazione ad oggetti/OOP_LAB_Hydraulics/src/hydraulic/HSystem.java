package hydraulic;

import java.util.ArrayList;
import java.util.List;

/**
 * Main class that act as a container of the elements for
 * the simulation of an hydraulics system 
 * 
 */
public class HSystem {
	
	protected List<Element> elements;
	
	public HSystem() {
		this.elements = new ArrayList<Element>();
	}
	
	/**
	 * Adds a new element to the system
	 * @param elem
	 */
	public void addElement(Element elem){
		this.elements.add(elem);
	}
	
	/**
	 * returns the element added so far to the system.
	 * If no element is present in the system an empty array (length==0) is returned.
	 * 
	 * @return an array of the elements added to the hydraulic system
	 */
	public Element[] getElements(){
		return this.elements.toArray(new Element[0]);
	}
	
	/**
	 * Prints the layout of the system starting at each Source
	 */
	public String layout(){
		boolean first = true;
		StringBuffer layout = new StringBuffer();
		for (Element e : this.elements) {
			if (e instanceof Source) {
				if (!first) {
					layout.append("\n");
				}
				layout.append(((Source)e).layout());
				if (first) {
					first = false;
				}
			}
		}
		return layout.toString();
	}
	
	/**
	 * starts the simulation of the system
	 */
	public void simulate(SimulationObserver observer){
		for (Element e : this.elements) {
			if (e instanceof Source) {
				((Source)e).simulate(observer);
			}
		}
	}

}
