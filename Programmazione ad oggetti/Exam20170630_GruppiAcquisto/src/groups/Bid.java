package groups;

public class Bid {
	
	private String supp;
	private int price;
	private int votes;
	private String product;

	public Bid(String supp, int price) {
		super();
		this.supp = supp;
		this.price = price;
		this.votes = 0;
	}

	public String getSupp() {
		return supp;
	}

	public void setSupp(String supp) {
		this.supp = supp;
	}

	public int getPrice() {
		return price;
	}

	public void setPrice(int price) {
		this.price = price;
	}
	
	public int getVotes() {
		return votes;
	}

	public void addVote() {
		this.votes += 1;
	}

	public String getProduct() {
		return product;
	}

	public void setProduct(String product) {
		this.product = product;
	}
	
	

}
