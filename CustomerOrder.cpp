

#include <iostream>
#include "Utilities.h"
#include "CustomerOrder.h"
#include <iomanip>

namespace sdds {
	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder()
	{
		m_name = "";
		m_product = "";
		m_cntItem = 0;
		m_listItem = nullptr;
		m_widthField = 0;
	}
	CustomerOrder::CustomerOrder(const std::string& str) {
		Utilities util;
		auto more = true;
		size_t next_pos = 0;

		m_name = util.extractToken(str, next_pos, more);
		m_product = util.extractToken(str, next_pos, more);
		m_cntItem = 0;

		auto start_pos = next_pos;

		while (more)
		{
			util.extractToken(str, next_pos, more);
			m_cntItem++;
		}

		m_listItem = new Item * [m_cntItem];

		more = true;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			m_listItem[i] = new Item(util.extractToken(str, start_pos, more));
		}

		if (m_widthField < util.getFieldWidth())
		{
			m_widthField = util.getFieldWidth();
		}
	}
	CustomerOrder::CustomerOrder(CustomerOrder&) {
		throw "error";
	}
	CustomerOrder::CustomerOrder(CustomerOrder&& co) noexcept {
		*this = std::move(co);
	}
	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept {
		if (this != &src) {
			m_name = src.m_name;
			m_product = src.m_product;
			m_cntItem = src.m_cntItem;
			m_listItem = src.m_listItem;

			src.m_listItem = { nullptr };
		}
		return *this;
	}
	CustomerOrder::~CustomerOrder() {
		if (m_listItem)
			for (unsigned int i = 0; i < m_cntItem; i++)
				delete m_listItem[i];

		delete[] m_listItem;
		m_listItem = nullptr;
	}
	bool CustomerOrder::isFilled() const {
		for (unsigned int i = 0; i < m_cntItem; i++)
			if (!m_listItem[i]->m_isFilled)
				return false;

		return true;
	}
	bool CustomerOrder::isItemFilled(const std::string& itemName) const {
		for (unsigned int i = 0; i < m_cntItem; i++)
			if (m_listItem[i]->m_itemName == itemName)
				return m_listItem[i]->m_isFilled;

		return true;
	}
	void CustomerOrder::fillItem(Station& station, std::ostream& os) {
		for (unsigned int i = 0; i < m_cntItem; i++)
		{
			if (m_listItem[i]->m_itemName == station.getItemName())
			{
				if (station.getQuantity() <= 0)
					os << "Unable to fill " << m_name << ", " << m_product << "[" << m_listItem[i]->m_itemName << "]" << std
					::endl;
				else
				{
					m_listItem[i]->m_serialNumber = station.getNextSerialNumber();
					station.updateQuantity();
					m_listItem[i]->m_isFilled = true;

					os << "Filled " << m_name << ", " << m_product << "[" << m_listItem[i]->m_itemName << "]" << std::endl;
				}

				return;
			}
		}
	}
	void CustomerOrder::display(std::ostream& os) const {
		os << m_name << " - " << m_product << std::endl;
		for (unsigned int i = 0; i < m_cntItem; i++)
		{
			os << "[" << std::setw(6) << std::setfill('0') << m_listItem[i]->m_serialNumber
				<< std::setfill(' ') << "] " << std::left << std::setw(m_widthField) << m_listItem[i]->m_itemName
				<< " - " << (m_listItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
		}
	}
}