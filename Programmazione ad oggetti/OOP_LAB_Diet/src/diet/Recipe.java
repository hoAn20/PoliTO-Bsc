package diet;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

/**
 * Represents a recipe of the diet.
 * 
 * A recipe consists of a a set of ingredients that are given amounts of raw materials.
 * The overall nutritional values of a recipe can be computed
 * on the basis of the ingredients' values and are expressed per 100g
 * 
 *
 */
public class Recipe implements NutritionalElement {
	
	class Ingredient {
		
		String material;
		double quantity;
		
		public Ingredient(String material, double quantity) {
			this.material = material;
			this.quantity = quantity;
		}
		
		@Override
		public String toString() {
			return this.material + " : " + String.format("%.1f", this.quantity) + "\n";
		}
		
	}
	
	private String name;
	private Map<String, NutritionalElement> materials;
	private List<Ingredient> ingredients;
	private double weight;

	public Recipe(String name, Map<String, NutritionalElement> rawMaterials) {
		this.name = name;
		this.materials = rawMaterials;
		this.ingredients = new LinkedList<>();
	}

	/**
	 * Adds a given quantity of an ingredient to the recipe.
	 * The ingredient is a raw material.
	 * 
	 * @param material the name of the raw material to be used as ingredient
	 * @param quantity the amount in grams of the raw material to be used
	 * @return the same Recipe object, it allows method chaining.
	 */
	public Recipe addIngredient(String material, double quantity) {
		this.ingredients.add(new Ingredient(material, quantity));
		this.weight += quantity;
		
		return this;
	}

	@Override
	public String getName() {
		return this.name;
	}

	@Override
	public double getCalories() {
		double qta = 0.0;
		for (Ingredient i : this.ingredients) {
			qta += this.materials.get(i.material).getCalories() * i.quantity / this.weight;
		}
		return qta;
	}

	@Override
	public double getProteins() {
		double qta = 0.0;
		for (Ingredient i : this.ingredients) {
			qta += this.materials.get(i.material).getProteins() * i.quantity / this.weight;
		}
		return qta;
	}

	@Override
	public double getCarbs() {
		double qta = 0.0;
		for (Ingredient i : this.ingredients) {
			qta += this.materials.get(i.material).getCarbs() * i.quantity / this.weight;
		}
		return qta;
	}

	@Override
	public double getFat() {
		double qta = 0.0;
		for (Ingredient i : this.ingredients) {
			qta += this.materials.get(i.material).getFat() * i.quantity/ this.weight;
		}
		return qta;
	}

	/**
	 * Indicates whether the nutritional values returned by the other methods
	 * refer to a conventional 100g quantity of nutritional element,
	 * or to a unit of element.
	 * 
	 * For the {@link Recipe} class it must always return {@code true}:
	 * a recipe expresses nutritional values per 100g
	 * 
	 * @return boolean indicator
	 */
	@Override
	public boolean per100g() {
		return true;
	}
	
	
	/**
	 * Returns the ingredients composing the recipe.
	 * 
	 * A string that contains all the ingredients, one per per line, 
	 * using the following format:
	 * {@code "Material : ###.#"} where <i>Material</i> is the name of the 
	 * raw material and <i>###.#</i> is the relative quantity. 
	 * 
	 * Lines are all terminated with character {@code '\n'} and the ingredients 
	 * must appear in the same order they have been added to the recipe.
	 */
	@Override
	public String toString() {
		StringBuffer res = new StringBuffer();
		for (Ingredient i : this.ingredients) {
			res.append(i.toString());
		}
		return res.toString();
	}
}
