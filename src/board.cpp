#include "board.h"

bool Board::isValidated(const Point& p)
{
	if (getMass(p).getStatus() == Mass::WALL)
	{
		return false;
	}
	return true;
}

bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}

bool Board::find(const Point& start, const Point& goal)
{
	Mass &mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_[start.y()][start.x()].setStatus(Mass::START);
	mass_[goal.y()][goal.x()].setStatus(Mass::GOAL);

	//�I�[�v�����X�g�ɊJ�n�m�[�h��ǉ�
	open_list_.clear();
	open_list_.push_back(&mass_start);

	//�I�[�v�����X�g����ł͂Ȃ�
	while (!open_list_.empty())
	{
		//���݂̃m�[�h=�I�[�v�����X�g�̍ł������ȃ��X�g
		std::sort(open_list_.begin(), open_list_.end(),asc);
		auto it = open_list_.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL)
		{
			//�ڕW�n�Ȃ�o�H�̊���
			Mass* p = current; //�o�H�̃X�e�[�^�X��Mass::WAYPOINT�ɂ���
			while (p)
			{
				if (p->getStatus() == Mass::BLANK)
				{
					p->setStatus(Mass::WAYPOINT);
				}

				p = p->getParent();
			}

			return true;
		}
		else
		{
			//���݂̃m�[�h���N���[�Y���X�g�Ɉڂ�
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//���݂̃m�[�h�̗אڂ���e�m�[�h�𒲂ׂ�
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			//�אڃm�[�h
			for (auto& c : next)
			{
				//�}�b�v�O�Ȃ�X�L�b�v
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y()) continue;
				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) && //�I�[�v�����X�g�Ɋ܂܂�Ă��Ȃ�
					!m.isListed(Mass::CLOSE) && //�N���[�Y�x���X�g�Ɋ܂܂�Ă��Ȃ�
					m.getStatus() != Mass::WALL)//��Q���łȂ�
				{
					//�I�[�v�����X�g�Ɏʂ��ăR�X�g���v�Z����
					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}

	return false;
}

void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "|";
			switch (mass_[y][x].getStatus()) {
			case Mass::BLANK:
				std::cout << " ";
				break;
			case Mass::START:
				std::cout << "S";
				break;
			case Mass::GOAL:
				std::cout << "G";
				break;
			case Mass::WAYPOINT:
				std::cout << "o";
				break;
			case Mass::WALL:
				std::cout << "#";
				break;
			case Mass::WATER:
				std::cout << "~";
				break;
			case Mass::ROAD:
				std::cout << "$";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;

}
