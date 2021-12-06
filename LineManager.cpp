
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <iostream>
#include "Workstation.h"
#include "LineManager.h"
#include "Utilities.h"
namespace sdds {

	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
	{
		try {
			m_cntCustomerOrder = pending.size();
			m_firstStation = nullptr;
			std::fstream readFile(file);
			if (!readFile) throw("");
			std::string temp;
			Utilities util;
			while (std::getline(readFile, temp)) {
				size_t next_pos = 0;
				bool more;
				std::string first, next;
				first = util.extractToken(temp, next_pos, more);
				if (more) next = util.extractToken(temp, next_pos, more);
				//Find first station
				std::for_each(stations.begin(), stations.end(), [&](Workstation* station) {
					if (station->getItemName() == first) {
						//Find next station
						std::for_each(stations.begin(), stations.end(), [&](Workstation* nextStation) {
							if (nextStation->getItemName() == next) {
								station->setNextStation(nextStation);
							}
							});
						activeLine.push_back(station);
					}
					});
				m_firstStation = activeLine.front();
			}
		}
		catch (...) {
			throw std::string("Error loading LineManager.");
		}
	}
	void LineManager::linkStations()
	{
		std::vector<Workstation*> sorted{ m_firstStation };
		size_t i = 0;
		std::for_each(activeLine.begin(), activeLine.end(), [&](Workstation* current) {
			std::for_each(activeLine.begin(), activeLine.end(), [&]
			(Workstation* next) {
					if (sorted[i]->getNextStation()) //check that next station
						if (sorted[i]->getNextStation()->getItemName() == next->getItemName()) {
							sorted.push_back(next);
							i++;
						}
				});
			});
		activeLine = sorted;
	}
	bool LineManager::run(std::ostream& os)
	{
		static size_t iterationNum = 0;
		size_t prevOrdersFinished = completed.size() + incomplete.size();
		os << "Line Manager Iteration: " << ++iterationNum << std::endl;
		if (!pending.empty()) {
			*m_firstStation += std::move(pending.front());
			pending.pop_front();
		}
		//fill orders
		std::for_each(activeLine.begin(), activeLine.end(), [&](Workstation*
			ws) {
				ws->fill(os);
			});
		//attempt to move orders
		std::for_each(activeLine.begin(), activeLine.end(), [&](Workstation*
			ws) {
				ws->attemptToMoveOrder();
			});
		//subtract difference from last iteration and this iteration
		m_cntCustomerOrder -= ((completed.size() + incomplete.size()) -
			prevOrdersFinished);
		return m_cntCustomerOrder <= 0;
	}
	void LineManager::display(std::ostream& os) const
	{
		std::for_each(activeLine.begin(), activeLine.end(), [&os](Workstation* ws) { ws->display(os); });
	}
}