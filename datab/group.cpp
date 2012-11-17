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


#include <QtCore/QDebug>
#include "group.h"

Group::Group(QObject *parent) : QObject(parent)
{
	name = QString();
	groupId = 0;
}

Group::Group(const Group &group) : QObject(group.parent())
{
	groupId = group.groupId;
	name = group.name;
	datesMap = group.datesMap;
	studentList = group.studentList;
}

Group::~Group()
{
	
}

void Group::addDate(qint32 disciplinId, const QString &date, quint32 numCriteria)
{
	QStringList ld = datesMap.value(disciplinId);
	if(ld.isEmpty())
		datesMap.insert(disciplinId, ld);
	
	datesMap[disciplinId].append(date);
	QVector<qreal> newRating(numCriteria);
	
	for(int i = 0; i < studentList.count(); i++)
	{
		studentList[i].addRating(disciplinId, newRating);
	}
}

QString Group::getDate(qint32 disciplinId, quint32 pos) const
{
	
	return datesMap.value(disciplinId).at(pos);
}

Student Group::getStudent(quint32 studentId) const
{
	return studentList.at(studentId);
}

Student *Group::getStudentPtr(quint32 studentId)
{
	return &studentList[studentId];
}

qreal Group::getAllRating(quint32 disciplinId, const QVector< qreal > &maxValues) const
{
	Student st;
	qreal res = 0.0;
	foreach(st, studentList)
	{
		res += st.getAllRating(disciplinId, maxValues);
	}
	return res;
}

qreal Group::getMidleRating(quint32 disciplinId, const QVector< qreal >& maxValues) const
{
	return (getAllRating(disciplinId, maxValues) / qreal(count()));
}

Group &Group::operator=(const Group &group)
{
	setParent(group.parent());
	groupId = group.groupId;
	name = group.name;
	datesMap = group.datesMap;
	studentList = group.studentList;
	return *this;
}

quint32 Group::getGroupId() const
{
	return groupId;
}

void Group::setGroupId(quint32 groupId)
{
	this->groupId = groupId;
}

quint32 Group::count() const
{
	return studentList.count();
}

void Group::setName(const QString &groupName)
{
	name = groupName;
}

QString Group::getName() const
{
	return name;
}

QStringList Group::getStudentNamesList() const
{
	QStringList res;
	Student st;
	foreach(st, studentList)
	{
		QString tmp = st.getName();
		res << tmp;
	}
	res.sort();
	return res;
}

QStringList Group::getStudentNamesIdList() const
{
	QStringList res;
	Student st;
	foreach(st, studentList)
	{
		res << st.getName() + QString("|%1").arg(st.getStudentId());
	}
	res.sort();
	return res;
}

void Group::addStudent(const Student &student, QVector<quint32> criteriaNums)
{
	quint32 studentId = studentList.count();
	studentList.append(student);
	studentList[studentId].setStudentId(studentId);
	
	for(int i = 0; i < criteriaNums.count(); i++)
	{
		QVector<qreal> vr(criteriaNums.at(i));
		for(int j = 0; j < datesMap.value(i).count(); j++)
			studentList[studentId].addRating(i, vr);
	}
}

quint32 Group::getDateCount(quint32 disciplinId) const
{
	return datesMap.value(disciplinId).count();
}

//===========================================================================

QDataStream &operator<<(QDataStream &out, const Group &gr)
{
	out << gr.groupId << gr.name << gr.datesMap << gr.studentList;
	return out;
}

QDataStream &operator>>(QDataStream &in, Group &gr)
{
	in >> gr.groupId;
	in >> gr.name;
	in >> gr.datesMap;
	in >> gr.studentList;
	return in;
}

#include "group.moc"