package transactions;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

//import static java.util.stream.Collectors.*;
//import static java.util.Comparator.*;

public class TransactionManager {
	
	private Map<String, Region> regions = new HashMap<String, Region>();
	private Map<String, Place> places = new HashMap<String, Place>();
	private Map<String, Carrier> carriers = new HashMap<String, Carrier>();
	private Map<String, Request> requests = new HashMap<String, Request>();
	private Map<String, Offer> offers = new HashMap<String, Offer>();
	private Map<String, Transaction> transactions = new HashMap<String, Transaction>();
	
//R1
	public List<String> addRegion(String regionName, String... placeNames) {
		Region r = new Region(regionName);
		regions.put(regionName, r);
		List<String> added = new LinkedList<String>();
		Stream.of(placeNames)
				.filter(s -> !places.containsKey(s))
				.forEach(s -> {
					places.put(s, new Place(s, r));
					added.add(s);
					added.sort(String::compareTo);
					});
		return added;
	}
	
	public List<String> addCarrier(String carrierName, String... regionNames) { 
		Carrier c = new Carrier(carrierName);
		carriers.put(carrierName, c);
		Stream.of(regionNames)
				.filter(s -> regions.containsKey(s))
				.distinct()
				.forEach(s -> c.addServedRegion(s));
		return c.getServedRegions().stream()
				.sorted(String::compareTo)
				.collect(Collectors.toList());
	}
	
	public List<String> getCarriersForRegion(String regionName) { 
		return carriers.values().stream()
				.filter(c -> c.getServedRegions().contains(regionName))
				.map(c -> c.getName())
				.sorted(String::compareTo)
				.collect(Collectors.toList());
	}
	
//R2
	public void addRequest(String requestId, String placeName, String productId) 
			throws TMException {
		if (!places.containsKey(placeName) || requests.containsKey(requestId))
			throw new TMException();
		requests.put(requestId, new Request(requestId, placeName, productId));
	}
	
	public void addOffer(String offerId, String placeName, String productId) 
			throws TMException {
		if (!places.containsKey(placeName) || offers.containsKey(offerId))
			throw new TMException();
		offers.put(offerId, new Offer(offerId, placeName, productId));
	}
	

//R3
	public void addTransaction(String transactionId, String carrierName, String requestId, String offerId) 
			throws TMException {
		List<String> alreadyRequested = transactions.values().stream()
				.map(t -> t.getRequestCode())
				.collect(Collectors.toList());
		List<String> alreadyOffered = transactions.values().stream()
				.map(t -> t.getOfferCode())
				.collect(Collectors.toList());
		if (alreadyOffered.contains(offerId) || alreadyRequested.contains(requestId))
			throw new TMException();
		Request r = requests.get(requestId);
		Offer o = offers.get(offerId);
		Carrier c = carriers.get(carrierName);
		if (r.getProductToSend().compareTo(o.getProductSendable()) != 0)
			throw new TMException();
		if (!c.getServedRegions().contains(places.get(r.getPlaceToReach()).getBelongingRegion().getName())
				||
			!c.getServedRegions().contains(places.get(o.getPlaceReachable()).getBelongingRegion().getName())
			)
				throw new TMException();
		
		transactions.put(transactionId, new Transaction(transactionId, carrierName, requestId, offerId));
	}
	
	public boolean evaluateTransaction(String transactionId, int score) {
		transactions.get(transactionId).setScore(score);
		return ((score > 0 && score < 11)?true:false);
	}
	
//R4
	public SortedMap<Long, List<String>> deliveryRegionsPerNT() {
		SortedMap<Long, List<String>> m = new TreeMap<Long, List<String>>(Collections.reverseOrder());
		transactions.values().stream()
				.map(t -> places.get(requests.get(t.getRequestCode()).getPlaceToReach()).getBelongingRegion().getName())
				.collect(Collectors.groupingBy(
							s -> s, 
							Collectors.counting()
						)
					).entrySet().stream()
						.forEach(e -> {
							if (!m.containsKey(e.getValue()))
								m.put(e.getValue(), new LinkedList<String>());
							m.get(e.getValue()).add(e.getKey());
						});
		return m;
	}
	
	public SortedMap<String, Integer> scorePerCarrier(int minimumScore) {
		SortedMap<String, Integer> m = new TreeMap<String, Integer>();
		transactions.values().stream()
				.filter(t -> t.getScore() >= minimumScore)
				.collect(Collectors.groupingBy(
							t -> t.getCarrier(),
							Collectors.summingInt(Transaction::getScore)
						)
					).entrySet().stream()
						.forEach(e -> {
							m.put(e.getKey(), e.getValue());
						});
		return m;
	}
	
	public SortedMap<String, Long> nTPerProduct() {
		return transactions.values().stream()
				.map(t -> offers.get(t.getOfferCode()).getProductSendable())
				.collect(Collectors.groupingBy(
							s-> s,
							TreeMap::new,
							Collectors.counting()
						)
					);
	}
	
	
}

