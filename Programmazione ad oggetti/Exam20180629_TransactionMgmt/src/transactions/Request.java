package transactions;

public class Request {
	
	private String code;
	private String placeToReach;
	private String productToSend;

	public Request(String requestId, String placeName, String productId) {
		this.code = requestId;
		this.placeToReach = placeName;
		this.productToSend = productId;
	}

	String getCode() {
		return code;
	}

	String getPlaceToReach() {
		return placeToReach;
	}

	String getProductToSend() {
		return productToSend;
	}
	
	

}
