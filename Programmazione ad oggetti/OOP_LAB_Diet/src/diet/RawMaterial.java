package diet;

public class RawMaterial implements NutritionalElement{
	
	private String name;
	private double cals;
	private double prots;
	private double carbs;
	private double fats;

	public RawMaterial(String name, double cals, double prots,
			double carbs, double fats) {
		
		this.name = name;
		this.cals = cals;
		this.prots = prots;
		this.carbs = carbs;
		this.fats = fats;
	}

	@Override
	public String getName() {
		return this.name;
	}

	@Override
	public double getCalories() {
		return this.cals;
	}

	@Override
	public double getProteins() {
		return this.prots;
	}

	@Override
	public double getCarbs() {
		return this.carbs;
	}

	@Override
	public double getFat() {
		return this.fats;
	}

	@Override
	public boolean per100g() {
		return true;
	}

}
