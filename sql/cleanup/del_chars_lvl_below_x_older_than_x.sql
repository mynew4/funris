DELETE FROM character_account_data WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_achievement WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_achievement_progress WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_action WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_arena_stats WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_aura WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_battleground_data WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_declinedname WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_equipmentsets WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_gifts WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_glyphs WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_homebind WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_instance WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));

DELETE FROM character_inventory WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM item_instance WHERE owner_guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM item_refund_instance WHERE player_guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_itemlog WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));

DELETE FROM character_pet WHERE owner IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_pet_declinedname WHERE owner IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_queststatus WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_queststatus_daily WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_queststatus_rewarded WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_queststatus_weekly WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_reputation WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_skills WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));

DELETE FROM character_social WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_social WHERE friend IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));

DELETE FROM character_spell WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_spell_cooldown WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_stats WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM character_talent WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM corpse WHERE player IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));

DELETE FROM arena_team_member WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM arena_team WHERE captainguid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM group_member WHERE memberGuid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM groups WHERE leaderGuid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM guild_member WHERE guid IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM mail WHERE receiver IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM mail_items WHERE receiver IN (SELECT guid FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90));
DELETE FROM characters WHERE level < 35 AND account IN (SELECT id FROM account WHERE DATEDIFF(NOW(), last_login) > 90);