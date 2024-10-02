DROP TABLE Category CASCADE CONSTRAINTS;
DROP TABLE Championship CASCADE CONSTRAINTS;
DROP TABLE ChampionshipEdition CASCADE CONSTRAINTS;
DROP TABLE Coach CASCADE CONSTRAINTS;
DROP TABLE Coaching CASCADE CONSTRAINTS;
DROP TABLE Place CASCADE CONSTRAINTS;
DROP TABLE Race CASCADE CONSTRAINTS;
DROP TABLE RaceResult CASCADE CONSTRAINTS;
DROP TABLE RaceType CASCADE CONSTRAINTS;
DROP TABLE Style CASCADE CONSTRAINTS;
DROP TABLE Swimmer CASCADE CONSTRAINTS;

CREATE TABLE Category (
  IDCategory  number(5) NOT NULL, 
  Description varchar2(100) NOT NULL, 
  PRIMARY KEY (IDCategory));

CREATE TABLE Championship (
  ChampionshipID number(10) NOT NULL, 
  Name           varchar2(255) NOT NULL, 
  PRIMARY KEY (ChampionshipID));

CREATE TABLE ChampionshipEdition (
  Code           number(10) NOT NULL, 
  ChampionshipID number(10) NOT NULL, 
  PlaceID        number(10) NOT NULL, 
  Year           number(10) NOT NULL, 
  Month          number(10) NOT NULL, 
  PRIMARY KEY (Code));

CREATE TABLE Coach (
  Code   number(10) NOT NULL, 
  Name   varchar2(255) NOT NULL, 
  Gender char(1) NOT NULL CHECK(Gender IN ('M', 'F')), 
  Phone  number(13) NOT NULL, 
  PRIMARY KEY (Code));

CREATE TABLE Coaching (
  SwimmerCode number(10) NOT NULL, 
  CoachCode   number(10) NOT NULL, 
  StartDate   date NOT NULL, 
  EndDate     date, 
  PRIMARY KEY (SwimmerCode, 
  CoachCode));

CREATE TABLE Place (
  PlaceID number(10) NOT NULL, 
  Name    varchar2(255) NOT NULL, 
  PRIMARY KEY (PlaceID));

CREATE TABLE Race (
  Code                    number(10) NOT NULL, 
  ChampionshipEditionCode number(10) NOT NULL, 
  StyleID                 number(4) NOT NULL, 
  Distance                number(4) NOT NULL, 
  PRIMARY KEY (Code));

CREATE TABLE RaceResult (
  SwimmerCode    number(10) NOT NULL, 
  RaceCode       number(10) NOT NULL, 
  Classification char(2) NOT NULL, 
  RaceTime       timestamp(0) NOT NULL, 
  PRIMARY KEY (SwimmerCode, 
  RaceCode));

CREATE TABLE RaceType (
  StyleID  number(4) NOT NULL, 
  Distance number(4) NOT NULL CHECK(Distance BETWEEN 50 and 2000), 
  Code     number(3) NOT NULL UNIQUE CHECK(code>0), 
  PRIMARY KEY (StyleID, 
  Distance));

CREATE TABLE Style (
  StyleID number(4) NOT NULL, 
  Name    varchar2(50) NOT NULL, 
  PRIMARY KEY (StyleID));

CREATE TABLE Swimmer (
  Code       number(10) NOT NULL, 
  Name       varchar2(255) NOT NULL, 
  Address    varchar2(255) NOT NULL, 
  BirthDate  date NOT NULL, 
  IDCategory number(5) NOT NULL, 
  PRIMARY KEY (Code));

ALTER TABLE Swimmer ADD CONSTRAINT FKSwimmer182597 FOREIGN KEY (IDCategory) REFERENCES Category (IDCategory);
ALTER TABLE ChampionshipEdition ADD CONSTRAINT FKChampionsh433900 FOREIGN KEY (ChampionshipID) REFERENCES Championship (ChampionshipID);
ALTER TABLE ChampionshipEdition ADD CONSTRAINT FKChampionsh411680 FOREIGN KEY (PlaceID) REFERENCES Place (PlaceID);
ALTER TABLE Race ADD CONSTRAINT FKRace456686 FOREIGN KEY (ChampionshipEditionCode) REFERENCES ChampionshipEdition (Code);
ALTER TABLE RaceResult ADD CONSTRAINT FKRaceResult377630 FOREIGN KEY (SwimmerCode) REFERENCES Swimmer (Code);
ALTER TABLE RaceResult ADD CONSTRAINT FKRaceResult251735 FOREIGN KEY (RaceCode) REFERENCES Race (Code);
ALTER TABLE Coaching ADD CONSTRAINT FKCoaching964042 FOREIGN KEY (SwimmerCode) REFERENCES Swimmer (Code);
ALTER TABLE Coaching ADD CONSTRAINT FKCoaching917721 FOREIGN KEY (CoachCode) REFERENCES Coach (Code);
ALTER TABLE RaceType ADD CONSTRAINT FKRaceType641056 FOREIGN KEY (StyleID) REFERENCES Style (StyleID);
ALTER TABLE Race ADD CONSTRAINT FKRace878877 FOREIGN KEY (StyleID, Distance) REFERENCES RaceType (StyleID, Distance);