package warehouse;

public class Order {
	
	private String code;
	private Product prod;
	private int quantity;
	private Supplier supp;
	private boolean delivered;

	public Order(String code, Product prod, int quantity, Supplier supp) {
		super();
		this.code = code;
		this.prod = prod;
		this.prod.setInOrder(this);
		this.quantity = quantity;
		this.supp = supp;
		this.delivered = false;
	}

	public String getCode(){
		// TODO: Completare!
		return this.code;
	}
	
	public boolean delivered(){
		// TODO: Completare!
		return this.delivered;
	}

	public void setDelivered() throws MultipleDelivery {
		// TODO: Completare!
		if (this.delivered)
			throw new MultipleDelivery();
		
		this.delivered = true;
		this.prod.increaseQty(this.quantity);
	}
	
	public String toString(){
		// TODO: Completare!
		return "Order "+this.code+" for "+this.quantity+" of "+this.prod.getCode()+
				" : "+this.prod.getDescription()+" form "+this.supp.getNome();
	}

	public int getQuantity() {
		// TODO Auto-generated method stub
		return this.quantity;
	}

	public Product getProduct() {
		// TODO Auto-generated method stub
		return this.prod;
	}

	public Supplier getSupplier() {
		// TODO Auto-generated method stub
		return this.supp;
	}
}
