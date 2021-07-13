package warehouse;

import java.util.LinkedList;
import java.util.List;

public class Supplier {
	
	private String code;
	private String name;
	private List<Product> products = new LinkedList<>();

	public Supplier(String code, String name) {
		super();
		this.code = code;
		this.name = name;
	}

	public String getCodice(){
		// TODO: completare!
		return this.code;
	}

	public String getNome(){
		// TODO: completare!
		return this.name;
	}
	
	public void newSupply(Product product){
		// TODO: completare!
		products.add(product);
		product.addSupplier(this);
	}
	
	public List<Product> supplies(){
		// TODO: completare!
		return this.products;
	}

	public boolean supplyes(Product prod) {
		// TODO Auto-generated method stub
		return products.contains(prod);
	}
}
