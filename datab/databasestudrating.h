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


#ifndef DATABASESTUDRATING_H
#define DATABASESTUDRATING_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include "group.h"
#include "criterion.h"

class DataBaseStudRating : public QObject
{
	Q_OBJECT
public:
	explicit DataBaseStudRating(QObject *parent = 0);
	DataBaseStudRating(const DataBaseStudRating &db);
	virtual ~DataBaseStudRating();

	void setCreateDate(QDate createDate);
	void setDescription(const QString &description);
	void addGroup(const Group &group);
	void addDisciplin(const QString &disciplin);
	void addCriterion(quint32 disciplinId, quint32 groupId, const QString &name, const qreal &value);
	void addStudent(quint32 groupId, const Student &student);

	QDate getCreateDate() const;
	QString getDescription() const;
	Group getGroup(quint32 groupId) const;
	QStringList getGroupNamesList() const;
	Group *getGroupPtr(quint32 groupId);
	QStringList getDisciplinList() const;
	QList<Criterion> getCriterion(quint32 disciplinId, quint32 groupId) const;
	QVector< qreal > getCriterionsValues(quint32 disciplinId, quint32 groupId) const;
	qreal getSumCriteria(quint32 disciplinId, quint32 groupId) const;

	quint32 countGroups() const;
	void editDisciplin(quint32 disciplinId, const QString &newDisciplinName);
	void editCriterion(quint32 disciplinId, quint32 groupId, quint32 criterionId, const QString &newName, const qreal &newValue);
	void editCriterionName(quint32 disciplinId, quint32 groupId, quint32 criterionId, const QString &newName);
	void editCriterionValue(quint32 disciplinId, quint32 groupId, quint32 criterionId, const qreal &newValue);

	void delDisciplin(quint32 disciplinId);
	void delGroup(quint32 groupId);
	
	void sentChanged();

	DataBaseStudRating &operator=(const DataBaseStudRating &db);
	friend QDataStream &operator<<(QDataStream &out, const DataBaseStudRating &db);
	friend QDataStream &operator>>(QDataStream &in, DataBaseStudRating &db);

signals:
	void dbChanged();
	
private:
	QDate createDate; // дата создания базы
	QString description; // описание базы
	QStringList disciplins; // список дисциплин
	QList<Group> groups; // список групп
	QMap<quint32, QMap<quint32, QList<Criterion> > > criteriaList; // список максимальных значений критериев для каждой группы
};

#endif // DATABASESTUDRATING_H
