package ticketing;

import java.util.HashSet;
import java.util.Set;

public class Component {
	
	private String name;
	private Set<String> subComponents = new HashSet<>();
	private Component parent;

	public Component(String name) {
		this.name = name;
	}

	public Component(String name, Component parent) {
		this.name = name;
		this.parent = parent;
	}

	public void addSubComponent(String sub) {
		this.subComponents.add(sub);
	}

	public Set<String> getSubComponents() {
		return this.subComponents;
	}

	public Component getParent() {
		return parent;
	}

	public String getName() {
		return name;
	}

}
