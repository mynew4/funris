SET @MAPID := 631;

DELETE FROM `spell_scripts` WHERE `command` IN (9,11,12,13) AND `datalong` IN (SELECT `guid` FROM `gameobject` WHERE `map` = @MAPID);
DELETE FROM `event_scripts` WHERE `command` IN (9,11,12,13) AND `datalong` IN (SELECT `guid` FROM `gameobject` WHERE `map` = @MAPID);
DELETE FROM `quest_start_scripts` WHERE `command` IN (9,11,12,13) AND `datalong` IN (SELECT `guid` FROM `gameobject` WHERE `map` = @MAPID);
DELETE FROM `quest_end_scripts` WHERE `command` IN (9,11,12,13) AND `datalong` IN (SELECT `guid` FROM `gameobject` WHERE `map` = @MAPID);

DELETE FROM `gameobject` WHERE `map` = @MAPID;
DELETE FROM `gameobject_scripts` WHERE `id` NOT IN (SELECT `guid` FROM `gameobject`);

INSERT INTO `gameobject` (SELECT * FROM `tdb`.`gameobject`);


REPLACE INTO `gameobject_template` (SELECT * FROM `tdb`.`gameobject_template` WHERE `entry` IN (SELECT DISTINCT `id` FROM `tdb`.`gameobject`));
REPLACE INTO `gameobject_scripts` (SELECT * FROM `tdb`.`gameobject_scripts`);
REPLACE INTO `quest_start_scripts` (SELECT * FROM `tdb`.`quest_start_scripts`);
REPLACE INTO `quest_end_scripts` (SELECT * FROM `tdb`.`quest_end_scripts`);
REPLACE INTO `spell_scripts` (SELECT * FROM `tdb`.`spell_scripts`);
REPLACE INTO `event_scripts` (SELECT * FROM `tdb`.`event_scripts`);
