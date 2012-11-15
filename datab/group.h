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


#ifndef GROUP_H
#define GROUP_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "student.h"

class Group : public QObject
{
	Q_OBJECT
public:
	explicit Group(QObject *parent = 0);
	Group(const Group &group);
	virtual ~Group();
	
	void setName(const QString &groupName);
	void setGroupId(quint32 groupId);
	void addDate(qint32 disciplinId, const QString &date, quint32 numCriteria = 0);
	void addStudent(const Student &student, QVector<quint32> criteriaNums);
	
	QString getName() const;
	quint32 getGroupId() const;
	QString getDate(qint32 disciplinId, quint32 pos) const;
	Student getStudent(quint32 studentId) const;
	Student *getStudentPtr(quint32 studentId);
	qreal getAllRating(quint32 disciplinId, const QVector< qreal > &maxValues) const;
	qreal getMidleRating(quint32 disciplinId, const QVector< qreal > &maxValues) const;
	QStringList getStudentNamesList() const;
	quint32 getDateCount(quint32 disciplinId) const;
	
	quint32 count() const;
	
	Group &operator=(const Group &group);
	friend QDataStream &operator<<(QDataStream &out, const Group &gr);
	friend QDataStream &operator>>(QDataStream &in, Group &gr);

private:
	quint32 groupId;
	QString name;
	QMap<quint32, QStringList> datesMap;
	QList<Student> studentList;
};

#endif // GROUP_H
