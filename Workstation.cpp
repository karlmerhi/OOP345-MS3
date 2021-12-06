
#include <iostream>
#include <iomanip>
#include "Workstation.h"



namespace sdds {

	std::deque<sdds::CustomerOrder> pending{};
	std::deque<sdds::CustomerOrder> completed{};
	std::deque<sdds::CustomerOrder> incomplete{};

	Workstation::Workstation()
	{
		m_pNextStation = nullptr;
	}
	Workstation::Workstation(const std::string& str) : Station(str)
	{
		m_pNextStation = nullptr;
	}
	void Workstation::fill(std::ostream& os)
	{
		if (!m_orders.empty())
			m_orders.front().fillItem(*this, os);
	}
	bool Workstation::attemptToMoveOrder()
	{
		if (m_orders.size() > 0) {
			if (m_orders.front().isItemFilled(getItemName()) || getQuantity()
				<= 0) {
				if (m_pNextStation) {
					*m_pNextStation += std::move(m_orders.front());
					m_orders.pop_front();
					return true;
				}
				else {
					if (m_orders.front().isFilled())
						completed.push_back(std::move(m_orders.front()));
					else
						incomplete.push_back(std::move(m_orders.front()));
					m_orders.pop_front();
					return true;
				}
			}
		}
		return false;
	}
	void Workstation::setNextStation(Workstation* station = nullptr)
	{
		m_pNextStation = station;
	}

		Workstation * Workstation::getNextStation() const
	{
		return m_pNextStation;
	}
	void Workstation::display(std::ostream& os) const
	{
		os << getItemName() << " --> "
			<< (m_pNextStation ? m_pNextStation->getItemName() : "End of Line") << std::endl;
	}
	Workstation& Workstation::operator+=(CustomerOrder&& newOrder)
	{
		m_orders.push_back(std::move(newOrder));
		return *this;
	}
}