package transactions;

public class Transaction {
	
	private String code;
	private String carrier;
	private String requestCode;
	private String offerCode;
	private int score;
	
	public Transaction(String transactionId, String carrierName, String requestId, String offerId) {
		this.code = transactionId;
		this.carrier = carrierName;
		this.requestCode = requestId;
		this.offerCode = offerId;
	}

	String getCode() {
		return code;
	}

	String getCarrier() {
		return carrier;
	}

	String getRequestCode() {
		return requestCode;
	}

	String getOfferCode() {
		return offerCode;
	}

	int getScore() {
		return score;
	}

	void setScore(int score) {
		this.score = score;
	}
	
	

}
