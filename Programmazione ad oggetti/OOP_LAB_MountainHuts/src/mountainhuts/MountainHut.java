package mountainhuts;

import java.util.Optional;

/**
 * Represents a mountain hut.
 * 
 * It is linked to a {@link Municipality}
 *
 */
public class MountainHut {
	
	private String name;
	private Optional<Integer> altitude;
	private String category;
	private Integer bedNumber;
	private Municipality municipality;

	public MountainHut(String name, Integer altitude, String category, Integer bedsNumber, 
			Municipality municipality) {
		this.name = name;
		if (altitude == null) {
			this.altitude = null;
		} else {
			this.altitude = Optional.of(altitude);
		}
		this.category = category;
		this.bedNumber = bedsNumber;
		this.municipality = municipality;
	}

	/**
	 * Unique name of the mountain hut
	 * @return name
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Altitude of the mountain hut.
	 * May be absent, in this case an empty {@link java.util.Optional} is returned.
	 * 
	 * @return optional containing the altitude
	 */
	public Optional<Integer> getAltitude() {
		return this.altitude;
	}

	/**
	 * Category of the hut
	 * 
	 * @return the category
	 */
	public String getCategory() {
		return this.category;
	}

	/**
	 * Number of beds places available in the mountain hut
	 * @return number of beds
	 */
	public Integer getBedsNumber() {
		return this.bedNumber;
	}

	/**
	 * Municipality where the hut is located
	 *  
	 * @return municipality
	 */
	public Municipality getMunicipality() {
		return this.municipality;
	}
	
	public String getProvince() {
		return this.municipality.getProvince();
	}

}
