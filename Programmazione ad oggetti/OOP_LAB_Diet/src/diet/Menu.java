package diet;

import java.util.Map;

/**
 * Represents a complete menu.
 * 
 * It can be made up of both packaged products and servings of given recipes.
 *
 */
public class Menu implements NutritionalElement {
	
	private String name;
	private Map<String, NutritionalElement> products;
	private Map<String, NutritionalElement> recipes;
	private double cals = 0.0;
	private double prots;
	private double carbs;
	private double fats;
	
	public Menu(String name, Map<String, NutritionalElement> products, Map<String, NutritionalElement> recipes) {
		this.name = name;
		this.products = products;
		this.recipes = recipes;
	}

	/**
	 * Adds a given serving size of a recipe.
	 * 
	 * The recipe is a name of a recipe defined in the
	 * {@Link Food} in which this menu has been defined.
	 * 
	 * @param recipe the name of the recipe to be used as ingredient
	 * @param quantity the amount in grams of the recipe to be used
	 * @return the same Menu to allow method chaining
	 */
	public Menu addRecipe(String recipe, double quantity) {
		Recipe n = (Recipe)this.recipes.get(recipe);
		this.cals += n.getCalories() * quantity / Food.STANDARD_PORTION;
		this.prots += n.getProteins() * quantity / Food.STANDARD_PORTION;
		this.carbs += n.getCarbs() * quantity / Food.STANDARD_PORTION;
		this.fats += n.getFat() * quantity / Food.STANDARD_PORTION;
		
		return this;
	}

	/**
	 * Adds a unit of a packaged product.
	 * The product is a name of a product defined in the
	 * {@Link Food} in which this menu has been defined.
	 * 
	 * @param product the name of the product to be used as ingredient
	 * @return the same Menu to allow method chaining
	 */
	public Menu addProduct(String product) {
		Product n = (Product)this.products.get(product);
		this.cals += n.getCalories();
		this.prots += n.getProteins();
		this.carbs += n.getCarbs();
		this.fats += n.getFat();
		
		return this;
	}

	/**
	 * Name of the menu
	 */
	@Override
	public String getName() {
		return this.name;
	}

	/**
	 * Total KCal in the menu
	 */
	@Override
	public double getCalories() {
//		System.out.println(this.cals);
		return this.cals;
	}

	/**
	 * Total proteins in the menu
	 */
	@Override
	public double getProteins() {
//		System.out.println(this.prots);
		return this.prots;
	}

	/**
	 * Total carbs in the menu
	 */
	@Override
	public double getCarbs() {
//		System.out.println(this.carbs);
		return this.carbs;
	}

	/**
	 * Total fats in the menu
	 */
	@Override
	public double getFat() {
//		System.out.println(this.fats);
		return this.fats;
	}

	/**
	 * Indicates whether the nutritional values returned by the other methods
	 * refer to a conventional 100g quantity of nutritional element,
	 * or to a unit of element.
	 * 
	 * For the {@link Menu} class it must always return {@code false}:
	 * nutritional values are provided for the whole menu.
	 * 
	 * @return boolean 	indicator
	 */
	@Override
	public boolean per100g() {
		// nutritional values are provided for the whole menu.
		return false;
	}
}
