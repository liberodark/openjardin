BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS `unites` (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT
);
INSERT INTO `unites` (id,designation) VALUES (1,'UNITE'),
 (2,'HEURES'),
 (3,'JOURS'),
 (4,'FORFAIT'),
 (5,'KG'),
 (6,'METRES'),
 (7,'M²'),
 (8,'LITRES');
CREATE TABLE IF NOT EXISTS `type_de_tache` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`couleur`	TEXT,
	`forme`	INTEGER
);
INSERT INTO `type_de_tache` (id,designation,couleur,forme) VALUES (1,'Phases','#00ffff',6),
 (2,'Préparation des sols','#ffaa00',1),
 (3,'Labour','#aa5500',2),
 (4,'Préparation lit de semence','#aaaa7f',1),
 (5,'Semis','#ffff00',8),
 (6,'documentations','#00ff00',1),
 (7,'Fertilisation','#00ff00',2),
 (8,'Récolte','#ff5500',1),
 (9,'Fin de culture','#ff0000',5),
 (10,'Arrosages','#00ffff',5);
CREATE TABLE IF NOT EXISTS `type_de_moyen` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT
);
INSERT INTO `type_de_moyen` (id,designation,commentaires) VALUES (1,'Personnel','Personnel de l''entreprise'),
 (2,'Matériel',NULL),
 (3,'Prestations extérieures',NULL),
 (4,'Fournitures',NULL);
CREATE TABLE IF NOT EXISTS `tasks` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`depart`	TEXT,
	`fin`	TEXT,
	`duree`	INTEGER,
	`precedent`	INTEGER,
	`phase_parent`	INTEGER,
	`avancement`	INTEGER,
	`type`	INTEGER,
	`contrainte_date`	INTEGER,
	`id_culture`	INTEGER
);

CREATE TABLE IF NOT EXISTS `ressources` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`tache`	INTEGER,
	`cout_prevu`	NUMERIC,
	`cout_actualise`	NUMERIC
);

CREATE TABLE IF NOT EXISTS `moyens` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`cout_unitaire`	NUMERIC,
	`coordonnees`	INTEGER,
	`type`	INTEGER,
	`unite`	INTEGER
);

CREATE TABLE IF NOT EXISTS `liste_ressources` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`id_task`	INTEGER,
	`id_ressource`	INTEGER
);

CREATE TABLE IF NOT EXISTS `liste_moyens` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`id_ressource`	INTEGER,
	`id_moyen`	INTEGER,
	`quantite_prevu`	NUMERIC,
	`quantite_realise`	NUMERIC,
	`unite`	INTEGER
);

CREATE TABLE IF NOT EXISTS `coordonnees` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`societe`	TEXT,
	`commentaires`	TEXT,
	`nom`	TEXT,
	`prenom`	TEXT,
	`adresse`	TEXT,
	`code_postal`	TEXT,
	`ville`	TEXT,
	`pays`	TEXT,
	`telephone_fixe`	TEXT,
	`portable`	TEXT,
	`fax`	TEXT,
	`email`	TEXT
);
CREATE TABLE IF NOT EXISTS `parcelles` (
         `id`	INTEGER UNIQUE PRIMARY KEY,
         `designation`	TEXT,
         `mode_affichage` INTEGER
 );

DROP TABLE IF EXISTS `familles`;
CREATE TABLE IF NOT EXISTS `familles` (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar ( 30 ),
        `couleur`	TEXT
);
INSERT INTO `familles` (id,designation,couleur) VALUES (1,'Actinidiacées','#ded8d7'),
 (2,'Amaranthacées','#ded8d7'),
 (3,'Apiacées
(Ombellifères)','#ff5500'),
 (4,'Astéracées (composées)','#aaaa7f'),
 (5,'Aizoaceae','#ded8d7'),
 (6,'Basellacées','#ded8d7'),
 (7,'Brassicacées
(Crucifères)','#aaffff'),
 (8,'Chénopodiacées','#ff557f'),
 (9,'Convolvulacées','#ded8d7'),
 (10,'Cucurbitacées','#ffaa7f'),
 (11,'Ericaceae','#ded8d7'),
 (12,'Fabacées
(Papilionacées)','#00ff00'),
 (13,'Grossulariacées','#ded8d7'),
 (14,'Lamiacées (Labiées)','#ded8d7'),
 (15,'Alliacées','#ffff7f'),
 (16,'Asparagacées
(Liliacées)','#ded8d7'),
 (17,'Poacées
(Graminées)','#aaaaff'),
 (18,'Polygonacées','#ded8d7'),
 (19,'Portulacacées','#ded8d7'),
 (20,'Rosacées','#ded8d7'),
 (21,'Solanacées','#ff00ff'),
 (22,'Valérianacées','#55ff7f'),
 (23,'Iridacée','#ded8d7'),
 (24,'-','#ded8d7'),
 (25,'Amaryllidaceae','#ded8d7'),
 (26,'Malvacées','#ded8d7'),
 (27,'Buxaceae','#ded8d7');

COMMIT;
