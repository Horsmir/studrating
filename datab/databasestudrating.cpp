/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Роман Браун <firdragon76@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "databasestudrating.h"

DataBaseStudRating::DataBaseStudRating(QObject *parent): QObject(parent)
{
	createDate = QDate::currentDate();
	description = QString();
}

DataBaseStudRating::DataBaseStudRating(const DataBaseStudRating &db): QObject(db.parent())
{
	createDate = db.createDate;
	description = db.description;
	disciplins = db.disciplins;
	groups = db.groups;
	criteriaList = db.criteriaList;
}

DataBaseStudRating::~DataBaseStudRating()
{

}

DataBaseStudRating &DataBaseStudRating::operator=(const DataBaseStudRating &db)
{
	setParent(db.parent());
	createDate = db.createDate;
	description = db.description;
	disciplins = db.disciplins;
	groups = db.groups;
	criteriaList = db.criteriaList;
	return *this;
}

void DataBaseStudRating::addDisciplin(const QString &disciplin)
{
	disciplins.append(disciplin);
}

void DataBaseStudRating::addGroup(const Group &group)
{
	groups.append(group);
	emit dbChanged();
}

quint32 DataBaseStudRating::countGroups() const
{
	return groups.count();
}

void DataBaseStudRating::delDisciplin(quint32 disciplinId)
{

}

void DataBaseStudRating::delGroup(quint32 groupId)
{

}

void DataBaseStudRating::editDisciplin(quint32 disciplinId, const QString &newDisciplinName)
{
	disciplins.replace(disciplinId, newDisciplinName);
}

QDate DataBaseStudRating::getCreateDate() const
{
	return createDate;
}

QString DataBaseStudRating::getDescription() const
{
	return description;
}

QStringList DataBaseStudRating::getDisciplinList() const
{
	return disciplins;
}

Group DataBaseStudRating::getGroup(quint32 groupId) const
{
	Q_ASSERT(groupId < groups.count());
	return groups.at(groupId);
}

QStringList DataBaseStudRating::getGroupNamesList() const
{
	QStringList res;
	Group gr;
	foreach(gr, groups)
		res << gr.getName();
	return res;
}

Group *DataBaseStudRating::getGroupPtr(quint32 groupId)
{
	Q_ASSERT(groupId < groups.count());
	return &groups[groupId];
}

void DataBaseStudRating::setCreateDate(QDate createDate)
{
	this->createDate = createDate;
}

void DataBaseStudRating::setDescription(const QString &description)
{
	this->description = description;
}

void DataBaseStudRating::addCriterion(quint32 disciplinId, quint32 groupId, const QString &name, const qreal &value)
{
	criteriaList[disciplinId][groupId].append(Criterion(name, value));
	if(groups.at(groupId).count() != 0)
	{
		for(int i = 0; i < groups.at(groupId).count(); i++)
			groups[groupId].getStudentPtr(i)->addOneEmptyRating(disciplinId);
	}
}

QList< Criterion > DataBaseStudRating::getCriterion(quint32 disciplinId, quint32 groupId) const
{
	return criteriaList.value(disciplinId).value(groupId);
}

void DataBaseStudRating::addStudent(quint32 groupId, const Student &student)
{
	Q_ASSERT(groupId < groups.count());
	QVector<quint32> criteriaNums;
	for(int i = 0; i < disciplins.count(); i++)
	{
		criteriaNums << criteriaList.value(i).value(groupId).count();
	}
	
	groups[groupId].addStudent(student, criteriaNums);
}

void DataBaseStudRating::sentChanged()
{
	emit dbChanged();
}

void DataBaseStudRating::editCriterion(quint32 disciplinId, quint32 groupId, quint32 criterionId, const QString &newName, const qreal &newValue)
{
	Criterion *cr = &criteriaList[disciplinId][groupId][criterionId];
	cr->setName(newName);
	cr->setValue(newValue);
}

void DataBaseStudRating::editCriterionName(quint32 disciplinId, quint32 groupId, quint32 criterionId, const QString &newName)
{
	Criterion *cr = &criteriaList[disciplinId][groupId][criterionId];
	cr->setName(newName);
}

void DataBaseStudRating::editCriterionValue(quint32 disciplinId, quint32 groupId, quint32 criterionId, const qreal &newValue)
{
	Criterion *cr = &criteriaList[disciplinId][groupId][criterionId];
	cr->setValue(newValue);
}

QVector<qreal> DataBaseStudRating::getCriterionsValues(quint32 disciplinId, quint32 groupId) const
{
	QVector<qreal> res;
	QList<Criterion> cl = criteriaList.value(disciplinId).value(groupId);
	Criterion c;
	foreach(c, cl)
	{
		res << c.getValue();
	}
	return res;
}

//====================================================================================================

QDataStream &operator<<(QDataStream &out, const DataBaseStudRating &db)
{
	out << db.createDate << db.description << db.disciplins << db.groups << db.criteriaList;
	return out;
}

QDataStream &operator>>(QDataStream &in, DataBaseStudRating &db)
{
	in >> db.createDate;
	in >> db.description;
	in >> db.disciplins;
	in >> db.groups;
	in >> db.criteriaList;
	return in;
}

#include "databasestudrating.moc"