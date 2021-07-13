package transactions;

public class Offer {
	
	private String code;
	private String placeReachable;
	private String productSendable;

	public Offer(String offerId, String placeName, String productId) {
		this.code = offerId;
		this.placeReachable = placeName;
		this.productSendable = productId;
	}

	String getCode() {
		return code;
	}

	String getPlaceReachable() {
		return placeReachable;
	}

	String getProductSendable() {
		return productSendable;
	}
	
	

}
