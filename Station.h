

#include <string>
#ifndef SDDS_STATION_H
#define SDDS_STATION_H

namespace sdds {

	class Station {
		size_t m_stationId;
		std::string m_itemName;
		std::string m_stationDescription;
		size_t m_serialNumber;
		size_t m_itemStock;
		static size_t m_widthField;
		static size_t id_generator;
	public:
		Station();
		Station(const std::string& line);
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};
}

#endif // !SDDS_STATION_H