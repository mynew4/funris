SET @MAPID := 631;

ALTER TABLE `gameobject` ADD INDEX `id` (`id`);
ALTER TABLE `gameobject` ADD INDEX `map` (`map`) ;

DELETE FROM `gameobject` WHERE `map` <> @MAPID;
DELETE FROM `spell_scripts` WHERE `command` NOT IN (9,11,12,13);
DELETE FROM `spell_scripts` WHERE `datalong` NOT IN (SELECT `guid` FROM `gameobject`);
DELETE FROM `event_scripts` WHERE `command` NOT IN (9,11,12,13);
DELETE FROM `event_scripts` WHERE `datalong` NOT IN (SELECT `guid` FROM `gameobject`);
DELETE FROM `quest_start_scripts` WHERE `command` NOT IN (9,11,12,13);
DELETE FROM `quest_start_scripts` WHERE `datalong` NOT IN (SELECT `guid` FROM `gameobject`);
DELETE FROM `quest_end_scripts` WHERE `command` NOT IN (9,11,12,13);
DELETE FROM `quest_end_scripts` WHERE `datalong` NOT IN (SELECT `guid` FROM `gameobject`);
DELETE FROM `gameobject_scripts` WHERE `id` NOT IN (SELECT `guid` FROM `gameobject`);

DROP TRIGGER IF EXISTS `gameobject_after_update`;
CREATE TRIGGER `gameobject_after_update` AFTER UPDATE ON `gameobject`
FOR EACH ROW BEGIN
UPDATE `event_scripts` SET `datalong`=NEW.guid WHERE `datalong`=OLD.guid AND `command` IN (9,11,12,13);
UPDATE `game_event_gameobject` SET `guid`=NEW.guid WHERE `guid`=OLD.guid;
UPDATE `gameobject_scripts` SET `id`=NEW.guid WHERE `id`=OLD.guid;
UPDATE `gameobject_scripts` SET `datalong`=NEW.guid WHERE `datalong`=OLD.guid AND `command` IN (9,11,12,13);
UPDATE `quest_start_scripts` SET `datalong`=NEW.guid WHERE `datalong`=OLD.guid AND `command` IN (9,11,12,13);
UPDATE `quest_end_scripts` SET `datalong`=NEW.guid WHERE `datalong`=OLD.guid AND `command` IN (9,11,12,13);
UPDATE `spell_scripts` SET `datalong`=NEW.guid WHERE `datalong`=OLD.guid AND `command` IN (9,11,12,13);
END;
SELECT @MAPID * 1000 INTO @temp_var;
UPDATE `gameobject` SET `guid`=@MAPID * 1000 WHERE `guid`=0;
UPDATE `gameobject` SET `guid`=@temp_var:=@temp_var+1 ORDER BY `guid` ASC;
DROP TRIGGER IF EXISTS `gameobject_after_update`;