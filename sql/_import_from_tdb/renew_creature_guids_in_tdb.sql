SET @MAPID := 631;

ALTER TABLE `waypoint_data` ADD INDEX `idx_id` (`id`) ;
ALTER TABLE `creature_addon` ADD INDEX `idx_path` (`path_id`) ;
ALTER TABLE `creature_template`
ADD INDEX `idx_diff1` (`difficulty_entry_1`),
ADD INDEX `idx_diff2` (`difficulty_entry_2`),
ADD INDEX `idx_diff3` (`difficulty_entry_3`);

DELETE FROM `creature` WHERE `map` <> @MAPID;
DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `linked_respawn` WHERE `linkType` <> 0;
DELETE FROM `linked_respawn` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `linked_respawn` WHERE `linkedGuid` NOT IN (SELECT `guid` FROM `creature`);

DELETE FROM `creature_formations` WHERE `leaderGUID` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `creature_formations` WHERE `memberGUID` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `waypoint_data` WHERE `id` NOT IN (SELECT `path_id` FROM `creature_addon`);
DELETE FROM `smart_scripts` WHERE `source_type` <> 0 OR `entryorguid` > 0;
UPDATE `smart_scripts` SET `entryorguid`=`entryorguid`*(-1);
DELETE FROM `smart_scripts` WHERE `entryorguid` NOT IN (SELECT `guid` FROM `creature`);


DROP TRIGGER IF EXISTS `creature_after_update`;
CREATE TRIGGER `creature_after_update` AFTER UPDATE ON `creature`
FOR EACH ROW BEGIN
UPDATE `creature_addon` SET `guid`=NEW.guid WHERE `guid`=OLD.guid;
UPDATE `creature_formations` SET `leaderGUID`=NEW.guid WHERE `leaderGUID`=OLD.guid;
UPDATE `creature_formations` SET `memberGUID`=NEW.guid WHERE `memberGUID`=OLD.guid;
UPDATE `linked_respawn` SET `guid`=NEW.guid WHERE `guid`=OLD.guid;
UPDATE `linked_respawn` SET `linkedGuid`=NEW.guid WHERE `linkedGuid`=OLD.guid;
UPDATE `smart_scripts` SET `entryorguid`=NEW.guid WHERE `entryorguid`=OLD.guid;
END;
SELECT @MAPID * 1000 INTO @temp_var;
UPDATE `creature` SET `guid` = @MAPID * 1000 WHERE `guid`=0;
UPDATE `creature` SET `guid`=@temp_var:=@temp_var+1 ORDER BY `guid` ASC;
DROP TRIGGER IF EXISTS `creature_after_update`;

UPDATE `smart_scripts` SET `entryorguid`=`entryorguid`*(-1);

UPDATE `waypoint_data` SET `id` = (SELECT `guid` FROM `creature_addon` WHERE `creature_addon`.`path_id` = `waypoint_data`.`id`);
UPDATE `creature_addon` SET `path_id` = `creature_addon`.`guid` WHERE `creature_addon`.`path_id` <> 0;