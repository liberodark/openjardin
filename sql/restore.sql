BEGIN TRANSACTION;
CREATE TABLE "taches" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30)
);
INSERT INTO `taches` (id,designation) VALUES (1,'Préparation du sol béchage');
INSERT INTO `taches` (id,designation) VALUES (3,'Amendement');
INSERT INTO `taches` (id,designation) VALUES (5,'Semis');
INSERT INTO `taches` (id,designation) VALUES (6,'Plantations');
INSERT INTO `taches` (id,designation) VALUES (7,'Désherbage');
INSERT INTO `taches` (id,designation) VALUES (8,'Récolte');
INSERT INTO `taches` (id,designation) VALUES (9,'Jachère');
INSERT INTO `taches` (id,designation) VALUES (10,'Paillage');
INSERT INTO `taches` (id,designation) VALUES (11,'Traitement');
INSERT INTO `taches` (id,designation) VALUES (14,'Maladie');

CREATE TABLE "plantes" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(40),
        `type_lune`	integer,
        `espece`	INTEGER,
        `nom_latin`	varchar(50),
        `commentaires`	varchar(200),
        `semis_printemps`	varchar(24),
        `semis_ete`	varchar(24),
        `semis_automne`	varchar(24)
);
INSERT INTO `plantes` (id,designation,type_lune,espece,nom_latin,commentaires,semis_printemps,semis_ete,semis_automne) VALUES (1,'-',0,114,'N/A','N/A','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (3,'Radis de 18 jours',3,91,'Raphanus sativus
L.','','001111000000000111100000','000000110000000000011000','000000000000000000000000'),
 (4,'Aneth',1,4,'Anethum graveolens','','011100000000001110000000','000001110000000000000000','000000001100011000000000'),
 (5,'Ail ',3,1,'0','culture avec buttage, pas de fumure','000000000000000111000000','000000000000000000000000','000000001110000001110000'),
 (6,'Celeri',1,22,'Apium graveolens L.','celeri grand vert','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (7,'Courge spaguetti',4,36,'Cucurbita pepo L. Spaghetti','courge spaguetti jaune','011100000000000001110000','000000000000000000000000','000000000000000000000000'),
 (8,'Chicorée italienne rossa di firenze',1,26,'Cichorium intybus L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (9,'Courgette longue de Nice ',1,37,'Cucurbita moschata','courgette longue de Nice ','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (10,'Livèche',1,57,'Levisticum officinale','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (11,'Seigle',3,104,'Secale cereale L.','seigle (à germer)','000000000000000000000000','000000000000000000000000','000000000110000000011000'),
 (12,'Safran',2,113,'Crocus sativus L.','','000000000000000000000000','000000110000000000000110','000000000000000000000000'),
 (13,'Cassisier',3,21,'Ribes nigrum','','000000000000000000000000','000000000000000000000000','111000000111000000111000'),
 (14,'Persil frisé',1,76,'Petroselinum crispum ','','001111000000000001111000','000000000000000000000000','000000000000000000000000'),
 (15,'Tomate San Marzano',4,109,'Solanum lycopersicum L.','Variété italienne pour sauce - plein champ sans tuteur','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (16,'Aubergine',4,9,'Solanum melongena L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (17,'Poivron doux',4,85,'Capsicum annuum
L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (18,'Carottes Touchon',3,19,'Daucus Carota L.','Variété précoce aux racines parfaitement cylindrique droite et demi-longue, de couleur orange rougeâtre brillant et sans coeur. La carotte touchon est une carotte réputée pour ces qualités gustatives. Se cultive dans un sol aéré, frais, bien travaillé, même les sols argileux peuvent lui convenir. Les sols compact ne sont pas recommandés, les racines auront tendances à se mettre en fourchettes. Prévenir contre la mouche de la carotte. ','001111000000000000011110','000000100000000000000001','000000000000000000000000'),
 (20,'Cornichon',1,35,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (21,'Artichaut Impérial Star',4,7,'Cynara cardunculus var. scolymus (L.)','Variété précoce et vigoureuse.
Pomme ronde d’un diamètre de 12 cm environ. Saveur douce à légèrement sucrée.','001100000000000000000110','000000000000000000000000','000000000110000110000000'),
 (22,'salade de saison',0,55,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (23,'basilic grand vert',0,12,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (24,'fraise des bois',0,49,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000');

CREATE TABLE "observations" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(40),
        `date`	varchar(10),
        `type`	INTEGER,
        `commentaires`	INTEGER,
        `id_culture`	INTEGER
);
CREATE TABLE "lune" (
        `id`	INTEGER,
        `designation`	varchar(30)
);
INSERT INTO `lune` (id,designation) VALUES (1,'Feuilles');
INSERT INTO `lune` (id,designation) VALUES (2,'Fleurs');
INSERT INTO `lune` (id,designation) VALUES (3,'Racines');
INSERT INTO `lune` (id,designation) VALUES (4,'Fruits / graines');

CREATE TABLE "familles" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30),
        `couleur`    varchar(10)
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

CREATE TABLE "especes" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30),
        `famille`	INTEGER,
        `positif`	varchar(200),
        `negatif`	varchar(200),
        `commentaires`	varchar(200),
        `compost`	INTEGER,
        `rotation_ans`	INTEGER,
        `rotation_familles`	varchar(22)
);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (1,'Ail',15,'carotte, fraisier, tomate','pois, haricot, chou','favorable aprés fabacées ou pomme de terre',1,6,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (2,'Airelle',11,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (3,'Amaranthe',2,'','',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (4,'Aneth',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (5,'Angélique',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (6,'Arroche',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (7,'Artichaut',3,'','','',3,6,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (8,'Asperge',16,'persil, tomate','oignon','',0,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (9,'Aubergine',21,'estragon, pois, thym','','souvent placé en tête de rotation',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (10,'Bambou',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (11,'Baselle',6,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (12,'Basilic',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (13,'Bette',8,'','','',2,5,'1000001000000010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (14,'Betterave',2,'choux, céleris, laitues, oignons','haricots grimpants','',2,5,'0010100010010010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (15,'Blé',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (16,'Blette',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (17,'Brocoli',7,'fève, céleri, camomille, aneth, menthe, capucine, oignon, origan, pomme de terre, sauge, romarin','laitue, fraise, tomate',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (18,'Cardon',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (19,'Carotte',3,'','','',2,6,'0000001000010100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (20,'Carvi',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (21,'Cassissier',13,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (22,'Céleri',3,'fève, choux, poireau, oignon, tomate','laitue, persil','',3,5,'0000001000010000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (23,'Céleri rave',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (24,'Cerfeuil',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (25,'Chayote',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (26,'Chicorée',4,'','','',2,5,'0000001000001000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (27,'Choux de Bruxelles',7,'fève, céleri, aneth, hysope, menthe, capucine, pomme de terre, sauge, romarin','fraise',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (28,'Choux',7,'fève, betterave, céleri, camomille, aneth, hysope, menthe, capucine, oignon, origan, pomme de terre, sauge, romarin','vigne, fraise, tomate','',3,5,'0000001000000100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (29,'Ciboule',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (30,'Ciboulette',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (31,'Citrouille',10,'fève, maïs, menthe, capucine, radis','pomme de terre',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (32,'Claytone',19,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (33,'Concombre',10,'fève, brocoli, céleri, laitue, pois, radis, tomate','rue, sauge','',3,5,'0000001000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (34,'Coriandre',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (35,'Cornichon',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (36,'Courge',10,'fève, maïs, menthe, capucine, radis','pomme de terre','',3,5,'0000000000000001000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (37,'Courgette',10,'','','',3,3,'0000001000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (38,'Cresson',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (40,'Crosne du japon',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (41,'Echalote',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (42,'Endive',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (43,'Epinard',8,'aubergine, chou, céleri, oignon, pois, fraise','','',3,5,'0000001000000010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (44,'Estragon',4,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (45,'Fenouil',3,'','','',3,5,'0000000000010000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (46,'Fenouil grand',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (47,'Fèves',12,'betteraves, bourrache, choux, carotte, chou-fleur, maïs, oeillet d''Inde, courge, fraise, tomate','ciboulette, fenouil, ail, poireau',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (48,'Cresson des Fontaines',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (49,'Fraisier',20,'','','',3,5,'0000001100000000100100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (50,'Framboisier',20,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (51,'Groseillier',13,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (52,'Haricot',12,'betteraves, bourrache, choux, carotte, chou-fleur, maïs, oeillet d''Inde, courge, fraise, tomate','ciboulette, fenouil, ail, poireau',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (53,'Hysope',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (54,'Kiwi',1,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (55,'Laitue',4,'betterave, chou, trèfle, pois, radis, fraise','','',2,5,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (56,'Lentille',12,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (57,'Livèche',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (58,'Maceron',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (59,'Mâche',22,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (60,'Mais doux',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (61,'Marjolaine',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (62,'Mélisse',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (63,'Melon',10,'maïs, tournesol','','',3,5,'0000000100000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (64,'Menthe',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (65,'Moutarde',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (66,'Myrtille',11,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (67,'Navet',7,'','','',1,4,'0000001110010100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (68,'Oignon',15,'betterave, chou, carotte, laitue, pomme de terre, fraise, tomate','fève, haricot, pois','',1,7,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (69,'Orge',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (70,'Origan',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (71,'Oseille',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (72,'Panais',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (73,'Pastèque',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (74,'Patate douce',9,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (75,'Pâtisson',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (76,'Persil',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (77,'Petits pois',12,'Les pois aiment la proximité du fenouil, des concombres, des laitues et des carottes.','','',2,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (78,'Physalis',21,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (79,'Piment',21,'basilic, carotte, marjolaine, oignons, origan','fenouil','',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (80,'Pois',12,'asperge, aubergine, carotte, céleri, concombres, laitue, radis, épinard, tomate, haricots, navets','','',1,5,'0000000000000000100000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (81,'Pissenlit',3,'',NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (82,'Poireau',25,'Carotte, céleri, endive, épinard, fraisier, mâche, œillet d''Inde, panais, tomate.','Ail, asperge, bette, betterave, brocoli, chou, chou de Bruxelles, chou-fleur, fève, haricot, oignon, persil, piment, pois, poivron, sauge.','Parasites   Anguillule, mouche de l''oignon, teigne du poireau, thrips, ver du poireau Maladies   Mildiou, oïdium, rouille',3,6,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (83,'Poirée',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (84,'Pois chiche',12,'l''artichaut, le brocoli, la carotte, le céleri, le chou, le concombre, la courge, l''épinard, la laitue, le melon, la menthe, le navet, le panais, le radis','l''ail, de l''asperge, la ciboulette, l''aubergine, l''oignon, le persil, le piment, le poireau, la pomme de terre, la tomate.','Le pois chiche redoute plus particulièrement la bruche du pois. Les grains sont transformés en gruyère, car la larve de la bruche, à la tête brune et au corps blanc, voire jaunâtre, creuse des galeries dans les grains. ',0,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (85,'Poivron',21,'','','',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (86,'Pomme de terre',21,'ail, haricot, pois, fève, chou, maïs, laitue, oignon, pétunia, oeillet d''Inde, radis, potiron, courgettes','pomme, citrouille, tomate','',3,5,'0000000000000000000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (87,'Potimarron',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (88,'Potiron',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (89,'Pourpier',19,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (90,'Quinoa',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (91,'Radis',7,'fève, chou, chou-fleur, concombre, laitue, pois, courge, tomate','vigne, hysope',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (92,'Raifort',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (93,'Rapini',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (94,'Rhubarbe',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (95,'Romarin',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (96,'Ronce (mûres)',20,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (97,'Roquette',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (98,'Rutabaga',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (99,'Salsifis',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (100,'Sariette',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (101,'Sarrasin',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (102,'Sauge',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (103,'Scorsonère',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (104,'Seigle',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (105,'Sorgho',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (106,'Stevia',4,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (107,'Tétragone',5,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (108,'Thym',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (109,'Tomate',21,'asperge, basilic, carotte, oignon, persil, pois, sauge, poireaux, radis','fenouil, pomme de terre, chou','',3,5,'0000000000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (110,'Tomate cerise',21,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (111,'Topinambour',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (112,'Tournesol',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (113,'Safran',23,'','','',2,5,'0000000000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (114,'-',24,'','','',1,1,'0000000000000000000000');
CREATE TABLE "unites" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30)
);
CREATE TABLE "type_de_tache" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`couleur`	varchar(10),
	`forme`	INTEGER
);
CREATE TABLE  "type_de_moyen" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200)
);
CREATE TABLE  "tasks" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
	`depart`	varchar(10),
	`fin`	varchar(10),
	`duree`	INTEGER,
	`precedent`	INTEGER,
	`phase_parent`	INTEGER,
	`avancement`	INTEGER,
	`type`	INTEGER,
	`contrainte_date`	INTEGER,
	`id_culture`	INTEGER
);
CREATE TABLE  "ressources" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
	`tache`	INTEGER,
	`cout_prevu`	NUMERIC,
	`cout_actualise`	NUMERIC
);

CREATE TABLE  "moyens" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
        `cout_unitaire`	NUMERIC,
	`coordonnees`	INTEGER,
	`type`	INTEGER,
	`unite`	INTEGER
);

CREATE TABLE  "liste_ressources" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`id_task`	INTEGER,
	`id_ressource`	INTEGER
);

CREATE TABLE  "liste_moyens" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`id_ressource`	INTEGER,
	`id_moyen`	INTEGER,
	`quantite_prevu`	NUMERIC,
	`quantite_realise`	NUMERIC,
	`unite`	INTEGER
);

CREATE TABLE "cultures" ( `id` INTEGER PRIMARY KEY AUTOINCREMENT,
`designation` varchar(40),
`parcelle` integer, `date_semis` varchar(10),
`type_plante` integer,
`commentaires` varchar(200),
`etat` INTEGER,
`duree` INTEGER,
`date_recolte` varchar(10) );

CREATE TABLE  "coordonnees" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`societe`	varchar(30),
	`commentaires`	varchar(200),
	`nom`	varchar(30),
	`prenom`	varchar(30),
	`adresse`	varchar(50),
	`code_postal`	varchar(5),
	`ville`	varchar(50),
	`pays`	varchar(20),
	`telephone_fixe`	varchar(20),
	`portable`	varchar(20),
	`fax`	varchar(20),
	`email` varchar(30)
);
INSERT INTO "unites" (id,designation) VALUES (1,'UNITE'),
 (2,'HEURES'),
 (3,'JOURS'),
 (4,'FORFAIT'),
 (5,'KG'),
 (6,'METRES'),
 (7,'M²'),
 (8,'LITRES');

INSERT INTO "type_de_tache" (id,designation,couleur,forme) VALUES (1,'Phases','#00ffff',6),
 (2,'Préparation des sols','#ffaa00',1),
 (3,'Labour','#aa5500',2),
 (4,'Préparation lit de semence','#aaaa7f',1),
 (5,'Semis','#ffff00',8),
 (6,'documentations','#00ff00',1),
 (7,'Fertilisation','#00ff00',2),
 (8,'Récolte','#ff5500',1),
 (9,'Fin de culture','#ff0000',5),
 (10,'Arrosages','#00ffff',5);
INSERT INTO "type_de_moyen" (id,designation,commentaires) VALUES (1,'Personnel','Personnel '),
 (2,'Matériel',' '),
 (3,'Prestations extérieures',' '),
 (4,'Fournitures',' ');

CREATE TABLE `parcelles` (
         `id`	INTEGER UNIQUE,
         `designation`	TEXT PRIMARY KEY,
         `mode_affichage` INTEGER
 );
COMMIT;
CREATE TABLE "parcelles" ( `id` INTEGER UNIQUE, `designation` TEXT, `type` INTEGER, PRIMARY KEY(`id`) )
