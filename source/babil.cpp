#include "babil.h"

#include "so_util.h"

namespace babil {

#define DECLARE(name) decltype(name) name = nullptr

DECLARE(initApp);
DECLARE(render);
DECLARE(touch);

DECLARE(fontScale);

DECLARE(OSi_Panic);
DECLARE(OS_Printf);

DECLARE(FX_Div);
DECLARE(NNS_G2dTickCellAnimation);
DECLARE(NNS_SndPlayerStopSeqBySeqArcIdx);

DECLARE(GX_SetWideMode);
DECLARE(G2_SetWnd0Position);

DECLARE(ScriptEngine::getByte);
DECLARE(ScriptEngine::getDword);
DECLARE(ScriptEngine::jump);
DECLARE(babilCommand_ClearCountJump);
DECLARE(babilCommand_CE_CameraPos);
DECLARE(babilCommand_CE_SetupCameraMotion);

DECLARE(texCount);
DECLARE(memCount);

DECLARE(LCD_WIDTH);
DECLARE(LCD_HEIGHT);

DECLARE(sys::GGlobal::getCurrentPart);
DECLARE(sys::GGlobal::setNextPart);

DECLARE(sys::GameParameter::isDash);
DECLARE(sys::GameParameter::setCloudGuideShowed);
DECLARE(sys::GameParameter::fieldSymbolID);
DECLARE(sys::GameParameter::setFieldSymbolID);
DECLARE(sys::GameParameter::gameClearCount);
DECLARE(sys::GameParameter::gold);
DECLARE(sys::GameParameter::gpInstance_);
DECLARE(sys::GameParameter::Ctor);
DECLARE(sys::GameParameter::gpInitialize);
DECLARE(sys::GameParameter::gpLoadDataReflect);
DECLARE(sys::GameParameter::previousSaveData);
DECLARE(sys::GameParameter::gpCopy);
DECLARE(sys::GameParameter::print);
DECLARE(sys::GameParameter::printMemory);
DECLARE(sys::GameParameter::getSaveNo);
DECLARE(sys::GameParameter::setSaveNo);
DECLARE(sys::GameParameter::getSaveNum);
DECLARE(sys::GameParameter::setSaveNum);
DECLARE(sys::GameParameter::playTime);
DECLARE(sys::GameParameter::setPlayTime);
DECLARE(sys::GameParameter::prevSaveForTime);
DECLARE(sys::GameParameter::reflectForTime);
DECLARE(sys::GameParameter::prevSaveForFlag);
DECLARE(sys::GameParameter::reflectForFlag);
DECLARE(sys::GameParameter::prevSaveForNavimap);
DECLARE(sys::GameParameter::prevSaveForNavimapAsync);
DECLARE(sys::GameParameter::reflectForNavimap2);
DECLARE(sys::GameParameter::setPartsHeaderStr);
DECLARE(sys::GameParameter::setDash);
DECLARE(sys::GameParameter::isCloudGuideShowed);
DECLARE(sys::GameParameter::setResetCloudGuideShowed);
DECLARE(sys::GameParameter::isShowConteEventTelop);
DECLARE(sys::GameParameter::isAutoMode);
DECLARE(sys::GameParameter::setAutoMode);
DECLARE(sys::GameParameter::isTVMode);
DECLARE(sys::GameParameter::setTVMode);
DECLARE(sys::GameParameter::getTVMode);
DECLARE(sys::GameParameter::isPlayVoice);
DECLARE(sys::GameParameter::incrementGameClearCount);
DECLARE(sys::GameParameter::coverNum);
DECLARE(sys::GameParameter::incrementCoverNum);
DECLARE(sys::GameParameter::formation);
DECLARE(sys::GameParameter::setFormation);
DECLARE(sys::GameParameter::changeFormation);
DECLARE(sys::GameParameter::item);
DECLARE(sys::GameParameter::rrc);
DECLARE(sys::GameParameter::abilityName);
DECLARE(sys::GameParameter::playerName);
DECLARE(sys::GameParameter::getMapName);
DECLARE(sys::GameParameter::setMapName);
DECLARE(sys::GameParameter::musicPlayerMode);
DECLARE(sys::GameParameter::monsterManiaManager);
DECLARE(sys::GameParameter::setMonsterManiaManager);
DECLARE(sys::GameParameter::playerSaveParameter);
DECLARE(sys::GameParameter::summonSaveParameter);
DECLARE(sys::GameParameter::getWorldPlayerPosition);
DECLARE(sys::GameParameter::setWorldPlayerPosition);
DECLARE(sys::GameParameter::getWorldPlayerRotation);
DECLARE(sys::GameParameter::setWorldPlayerRotation);
DECLARE(sys::GameParameter::getWorldVehiclePosition);
DECLARE(sys::GameParameter::setWorldVehiclePosition);
DECLARE(sys::GameParameter::getWorldVehicleRotation);
DECLARE(sys::GameParameter::setWorldVehicleRotation);
DECLARE(sys::GameParameter::getWorldVehicleStay);
DECLARE(sys::GameParameter::setWorldVehicleStay);
DECLARE(sys::GameParameter::getWorldTelepoPosition);
DECLARE(sys::GameParameter::setWorldTelepoPosition);
DECLARE(sys::GameParameter::getWorldTelepoRotation);
DECLARE(sys::GameParameter::setWorldTelepoRotation);
DECLARE(sys::GameParameter::getWorldTelepoMapName);
DECLARE(sys::GameParameter::setWorldTelepoMapName);
DECLARE(sys::GameParameter::getMagishipPosOffsetOnMoonChip);
DECLARE(sys::GameParameter::setMagishipPosOffsetOnMoonChip);
DECLARE(sys::GameParameter::getMagishipMoonChipX);
DECLARE(sys::GameParameter::setMagishipMoonChipX);
DECLARE(sys::GameParameter::getMagishipMoonChipZ);
DECLARE(sys::GameParameter::setMagishipMoonChipZ);
DECLARE(sys::GameParameter::getMagshipMoonPos);
DECLARE(sys::GameParameter::setMagshipMoonPos);
DECLARE(sys::GameParameter::getMagshipEarthPos);
DECLARE(sys::GameParameter::setMagshipEarthPos);
DECLARE(sys::GameParameter::getBlackChocoboHome);
DECLARE(sys::GameParameter::setBlackChocoboHome);
DECLARE(sys::GameParameter::getDesionMapName);
DECLARE(sys::GameParameter::setDesionMapName);
DECLARE(sys::GameParameter::getDesionPosition);
DECLARE(sys::GameParameter::setDesionPosition);
DECLARE(sys::GameParameter::getDesionRot);
DECLARE(sys::GameParameter::setDesionRot);
DECLARE(sys::GameParameter::getDesionDataValidity);
DECLARE(sys::GameParameter::setDesionDataValidity);
DECLARE(sys::GameParameter::getPassagePoint2);
DECLARE(sys::GameParameter::setPassagePoint2);

DECLARE(map2d::NAVI_MAP_TASK_NAME);
DECLARE(map2d::NaviMap::MapOn);
DECLARE(map2d::NaviMap::MapOff);
DECLARE(map2d::NaviMap::setAlpha);
DECLARE(map2d::NaviMap::NaviMapDrawOff);

DECLARE(movie::MoviePart::getInstance);
DECLARE(movie::MoviePart::setAfterPart);

DECLARE(ui::CWidgetMng::initialize);
DECLARE(ui::CWidgetMng::terminate);
DECLARE(ui::CWidgetMng::cleanWidget);
DECLARE(ui::CWidgetMng::addWidget);
DECLARE(ui::CWidgetMng::deleteWidget);
DECLARE(ui::CWidgetMng::deleteWidgetRange);
DECLARE(ui::CWidgetMng::findWidget);
DECLARE(ui::CWidgetMng::draw);
DECLARE(ui::CWidgetMng::drawWidget);
DECLARE(ui::CWidgetMng::execute);
DECLARE(ui::CWidgetMng::moveWidget);
DECLARE(ui::CWidgetMng::moveSlide);
DECLARE(ui::CWidgetMng::setWidgetHilight);
DECLARE(ui::CWidgetMng::setWidgetEnable);
DECLARE(ui::CWidgetMng::getActiveCursor);
DECLARE(ui::CWidgetMng::getActiveCursorItem);
DECLARE(ui::CWidgetMng::getActiveCursorListMax);
DECLARE(ui::CWidgetMng::getCursorItem);
DECLARE(ui::CWidgetMng::setCursor);
DECLARE(ui::CWidgetMng::setListMax);
DECLARE(ui::CWidgetMng::getListMax);
DECLARE(ui::CWidgetMng::setScroll);
DECLARE(ui::CWidgetMng::setScrollPos);
DECLARE(ui::CWidgetMng::setScrollRect);
DECLARE(ui::CWidgetMng::setSlide);
DECLARE(ui::CWidgetMng::setDefaultPadKeyFunc);
DECLARE(ui::CWidgetMng::setDragState);
DECLARE(ui::CWidgetMng::DefaultPadKeyFunc);
DECLARE(ui::CWidgetMng::DefaultPadKeyFunc_Line2);
DECLARE(ui::CWidgetMng::DefaultPadKeyFunc_Line3);
DECLARE(ui::CWidgetMng::DefaultPadKeyFunc_LineMulti);
DECLARE(ui::g_WidgetMng);

DECLARE(world::WorldState::wsGetString);
DECLARE(world::WorldState::wsSetEnd);

DECLARE(world::WorldStateScheduler::wssInitialize);
DECLARE(world::WorldStateScheduler::wssGetCurrentState);
DECLARE(world::WorldStateScheduler::wssAddStateSchedule);

DECLARE(world::WorldTask::queryTask);
DECLARE(world::WorldTask::deleteTask);

DECLARE(world::MSSConfig::mssInitialize);
DECLARE(world::MSSConfig::mssProcess);

DECLARE(world::WSMove::WSMove);
DECLARE(world::WSMove::wsmEncount);
DECLARE(world::WSMove::wsIsMenuOpen);

DECLARE(world::WSVehicleMove::wsProcess);

DECLARE(world::CCameraVibration::startVibration);
DECLARE(world::CCameraVibration::ccbUpdate);

DECLARE(world::enableSPSetting);
DECLARE(world::disableSPSetting);
DECLARE(world::searchNextSymbolByCharacterIDBackward);
DECLARE(world::searchNextSymbolByCharacterIDForward);
DECLARE(world::canChangeSymbol);
DECLARE(world::MSSTextScreenClear);

DECLARE(part::WorldPart::wpInstance_);

DECLARE(btl::BattlePart::doInitialize);
DECLARE(btl::BattlePart::instance_);

DECLARE(btl::battleSpeedRate);

DECLARE(btl::Battle2DManager::ctrlWidgetCheck);
DECLARE(btl::Battle2DManager::instance_);

DECLARE(btl::BattleMonsterParty::giftGold);
DECLARE(btl::BattleMonsterParty::giftExp);

DECLARE(btl::BattlePlayer::playerId);

DECLARE(btl::BattleCommandSelector::initialize);
DECLARE(btl::BattleCommandSelector::commandAction);

DECLARE(CCharacterMng::getIndexOfSearchingCharacter);
DECLARE(CCharacterMng::getCharacterName);
DECLARE(CCharacterMng::setHidden);
DECLARE(CCharacterMng::setShadowEnable);
DECLARE(CCharacterMng::setShadowType);
DECLARE(CCharacterMng::setShadowAlphaRate);
DECLARE(CCharacterMng::setTransparencyRate);
DECLARE(CCharacterMng::setPosition);
DECLARE(CCharacterMng::setRotation);
DECLARE(CCharacterMng::startMotion);
DECLARE(CCharacterMng::setPause);
DECLARE(CCharacterMng::setFrameRate);
DECLARE(characterMng);

DECLARE(ys::Condition::isNotBattleCondition);

DECLARE(pl::Player::condition);
DECLARE(pl::Player::Ctor);
DECLARE(pl::Player::Dtor);
DECLARE(pl::Player::initialize);
DECLARE(pl::Player::initializeName);
DECLARE(pl::Player::initializeCommand);
DECLARE(pl::Player::initializeDefaultCommand);
DECLARE(pl::Player::updateParameter);
DECLARE(pl::Player::updateCondition);
DECLARE(pl::Player::recoverHPandMP);
DECLARE(pl::Player::useApple);
DECLARE(pl::Player::useSomaDrop);
DECLARE(pl::Player::fine);
DECLARE(pl::Player::playerAbilityManager);
DECLARE(pl::Player::learningAbility);
DECLARE(pl::Player::equipParameter);
DECLARE(pl::Player::bodyAndBonus);
DECLARE(pl::Player::level);
DECLARE(pl::Player::hp);
DECLARE(pl::Player::mp);
DECLARE(pl::Player::exp);
DECLARE(pl::Player::decantLevel);
DECLARE(pl::Player::saveParameter);
DECLARE(pl::Player::growParameter);
DECLARE(pl::Player::levelUp);
DECLARE(pl::Player::levelDown);
DECLARE(pl::Player::setParameter);
DECLARE(pl::Player::setExp);
DECLARE(pl::Player::setHp);
DECLARE(pl::Player::bodyAndBonus_BP);
DECLARE(pl::Player::calcBonus);
DECLARE(pl::Player::equipmentBounsHP);
DECLARE(pl::Player::abilityBouns);
DECLARE(pl::Player::addEquipmentBonus);
DECLARE(pl::Player::setPhysicsAttack);
DECLARE(pl::Player::calcPhysicsDefense);
DECLARE(pl::Player::calcMagicDefense);
DECLARE(pl::Player::magicDefensePower);
DECLARE(pl::Player::physicsAttack);
DECLARE(pl::Player::physicsDefense);
DECLARE(pl::Player::magicDefense);
DECLARE(pl::Player::addMagicDefense);
DECLARE(pl::Player::addPhysicsDefense_ib);
DECLARE(pl::Player::addPhysicsDefense_Pb);
DECLARE(pl::Player::avoidance);
DECLARE(pl::Player::deftness);
DECLARE(pl::Player::defaultMaxHp);
DECLARE(pl::Player::defaultMaxMp);
DECLARE(pl::Player::calcHpColor);
DECLARE(pl::Player::checkHpColor);
DECLARE(pl::Player::isHealing);
DECLARE(pl::Player::difference);
DECLARE(pl::Player::canWinningDirection);
DECLARE(pl::Player::canGivenExp);
DECLARE(pl::Player::isFinishAttack);
DECLARE(pl::Player::isUseMagicCharacter);
DECLARE(pl::Player::isCommand);
DECLARE(pl::Player::isKnowledgeInCommand);
DECLARE(pl::Player::listupCommand);
DECLARE(pl::Player::listupCommandNormal);
DECLARE(pl::Player::listupCommandMagic);
DECLARE(pl::Player::clearCommand);
DECLARE(pl::Player::stonePaletteName);
DECLARE(pl::Player::changeStonePalette);
DECLARE(pl::Player::changeStonePaletteForEvent);
DECLARE(pl::Player::canEquip);
DECLARE(pl::Player::canEquipWeapon);
DECLARE(pl::Player::canEquipBow);
DECLARE(pl::Player::canEquipArrow);
DECLARE(pl::Player::canEquipHarp);
DECLARE(pl::Player::isCanEquipProtection);
DECLARE(pl::Player::isHandCheck);
DECLARE(pl::Player::isEquipable);
DECLARE(pl::Player::isEquipItem);
DECLARE(pl::Player::isEquippedBow);
DECLARE(pl::Player::isEquipedItemOnPoint);
DECLARE(pl::Player::isEquipedItemOnAllPoints);
DECLARE(pl::Player::isEquipMetal);
DECLARE(pl::Player::isHand);
DECLARE(pl::Player::doEquip);
DECLARE(pl::Player::equipmentAttach);
DECLARE(pl::Player::attachEquipmentSymbol);
DECLARE(pl::Player::attachEquipmentSymbolOne);
DECLARE(pl::Player::removeEquipmentSymbol);
DECLARE(pl::Player::removeEquipmentSymbolOne);
DECLARE(pl::Player::equipmentSymbolVisibility);
DECLARE(pl::Player::updateEquipmentSymbol);
DECLARE(pl::Player::releaseEquipItem);
DECLARE(pl::Player::adjustAbility);
DECLARE(pl::Player::equipMenuParameter);
DECLARE(pl::Player::spendMp);
DECLARE(pl::Player::isUseMagic_BM);
DECLARE(pl::Player::isUseMagic_AI);
DECLARE(pl::Player::learnAbility_ii);
DECLARE(pl::Player::learnAbility_AI);
DECLARE(pl::Player::forgetAbility_i);
DECLARE(pl::Player::forgetAbility_AI);
DECLARE(pl::Player::forgetLearnedMagicByEvent);
DECLARE(pl::Player::learnAbilityForDebug);
DECLARE(pl::Player::learnAllAbilityForDebug);
DECLARE(pl::Player::learnAllWhiteMagicForDebug);
DECLARE(pl::Player::learnAllBlackMagicForDebug);
DECLARE(pl::Player::learnAllSummonForDebug);
DECLARE(pl::Player::learnAllSongForDebug);
DECLARE(pl::Player::learnAllNinjutsuForDebug);
DECLARE(pl::Player::forgetAllAbilityForDebug);
DECLARE(pl::Player::forgetAllWhiteMagicForDebug);
DECLARE(pl::Player::forgetAllBlackMagicForDebug);
DECLARE(pl::Player::forgetAllSummonForDebug);
DECLARE(pl::Player::forgetAllSongForDebug);
DECLARE(pl::Player::forgetAllNinjutsuForDebug);

DECLARE(pl::PlayerParty::member);
DECLARE(pl::PlayerParty::memberOrder);
DECLARE(pl::PlayerParty::memberForOrder);
DECLARE(pl::PlayerParty::player);
DECLARE(pl::PlayerParty::isJoinedParty);
DECLARE(pl::PlayerParty::isPartyFull);
DECLARE(pl::PlayerParty::isEnableMember);
DECLARE(pl::PlayerParty::partyMemberEnableNumber);
DECLARE(pl::PlayerParty::aliveNumber);
DECLARE(pl::PlayerParty::averageLevel);
DECLARE(pl::PlayerParty::isFrogAll);
DECLARE(pl::PlayerParty::isLilliputAll);
DECLARE(pl::PlayerParty::saveComposition);
DECLARE(pl::PlayerParty::loadComposition);
DECLARE(pl::PlayerParty::setMemberForOrder);
DECLARE(pl::PlayerParty::changeMemberForOrder);
DECLARE(pl::PlayerParty::changeMember);
DECLARE(pl::PlayerParty::addMember);
DECLARE(pl::PlayerParty::releaseMember);
DECLARE(pl::PlayerParty::Ctor);
DECLARE(pl::PlayerParty::Dtor);
DECLARE(pl::PlayerParty::initialize);
DECLARE(pl::PlayerParty::initForNewgame);
DECLARE(pl::PlayerParty::initializeName);
DECLARE(pl::PlayerParty::load);
DECLARE(pl::PlayerParty::free);
DECLARE(pl::PlayerParty::fineAll);
DECLARE(pl::PlayerParty::changeStonePalette);
DECLARE(pl::PlayerParty::changeStonePaletteForEvent);
DECLARE(pl::PlayerParty::clearBattleCommand);
DECLARE(pl::PlayerParty::clearBattleCommandPlayer);
DECLARE(pl::PlayerParty::setSaveStartCommand);
DECLARE(pl::PlayerParty::isCommand);
DECLARE(pl::PlayerParty::listupCommand);
DECLARE(pl::PlayerParty::clearBattleCondition);
DECLARE(pl::PlayerParty::addItem);
DECLARE(pl::PlayerParty::formation);
DECLARE(pl::PlayerParty::setFormation);
DECLARE(pl::PlayerParty::changeFormation);
DECLARE(pl::PlayerParty::levelParameter);
DECLARE(pl::PlayerParty::learningAbility);
DECLARE(pl::PlayerParty::isLearnByLevelUp);
DECLARE(pl::PlayerParty::normalMagic);
DECLARE(pl::PlayerParty::normalAttack);
DECLARE(pl::PlayerParty::abilityList);
DECLARE(pl::PlayerParty::layoutSceneParameter);
DECLARE(pl::PlayerParty::weaponOffsetParameter);
DECLARE(pl::PlayerParty::abilityBouns);
DECLARE(pl::PlayerParty::playerPartyInstance_);
DECLARE(pl::PlayerParty::InvalidPlayer);

DECLARE(menu::playSECommon);

DECLARE(CoreAudioOutInit);
DECLARE(CoreAudioOutKick);
DECLARE(CoreAudioOutQueueBuffer);
DECLARE(CoreAudioOutRelease);
DECLARE(CoreAudioOutResume);
DECLARE(CoreAudioOutSuspend);
DECLARE(NNS_SndUpdate);
DECLARE(NNS_SndPlayerSetPlayerVolume);

DECLARE(ds::snd::BGMHandle::Ctor);
DECLARE(ds::snd::BGMHandle::Dtor);
DECLARE(ds::snd::BGMHandle::IsPlaying);
DECLARE(ds::snd::BGMHandle::PlayV);
DECLARE(ds::snd::BGMHandle::Play);
DECLARE(ds::snd::BGMHandle::SetVolume);
DECLARE(ds::snd::BGMHandle::MoveVolume);
DECLARE(ds::snd::BGMHandle::Stop);
DECLARE(ds::snd::BGMHandle::seqNo);
DECLARE(ds::snd::BGMHandle::Pause);
DECLARE(ds::snd::BGMHandle::setBGMEnable);
DECLARE(ds::snd::BGMHandle::getBGMEnable);
DECLARE(ds::snd::BGMHandle::forceReleaseSequence);
DECLARE(ds::snd::BGMHandle::setTempo);

DECLARE(ds::snd::SEHandle::Ctor);
DECLARE(ds::snd::SEHandle::Dtor);
DECLARE(ds::snd::SEHandle::IsPlaying);
DECLARE(ds::snd::SEHandle::Play);
DECLARE(ds::snd::SEHandle::PlayFadeIn);
DECLARE(ds::snd::SEHandle::setPan);
DECLARE(ds::snd::SEHandle::setPanRange);
DECLARE(ds::snd::SEHandle::setPitch);
DECLARE(ds::snd::SEHandle::setTempo);
DECLARE(ds::snd::SEHandle::setVolume);
DECLARE(ds::snd::SEHandle::MoveVolume);
DECLARE(ds::snd::SEHandle::stop);

DECLARE(ds::sys3d::CAnimation::ctor);
DECLARE(ds::sys3d::CAnimation::getFrameRate);
DECLARE(ds::sys3d::CAnimation::setup);
DECLARE(ds::sys3d::CAnimation::setFrameRate);
DECLARE(ds::sys3d::CMotSet::getFrameRate);
DECLARE(ds::sys3d::CMotSet::setFrameRate);

DECLARE(ds::CPad::repeat);
DECLARE(ds::g_Pad);

DECLARE(egs::GilbartInitialize);
DECLARE(egs::GilbartTerminate);
DECLARE(egs::GilbartIsPlaying);
DECLARE(egs::GilbartIsEndOfPlayVoice);
DECLARE(egs::GilbartSetVolume);
DECLARE(egs::GilbartPauseVoice);
DECLARE(egs::GilbartPlayVoice);
DECLARE(egs::GilbartStopVoice);
DECLARE(egs::GilbartStatus);
DECLARE(egs::GilbartProcess);

DECLARE(Font::initFont);

DECLARE(dgs::DGSMsdGetString);

DECLARE(title::TitleContents::update);

DECLARE(title::TitleSubState::update);

DECLARE(FlagManager::singleton);
DECLARE(FlagManager::Ctor);
DECLARE(FlagManager::Dtor);
DECLARE(FlagManager::get);
DECLARE(FlagManager::set);
DECLARE(FlagManager::reset);
DECLARE(FlagManager::reverse);
DECLARE(FlagManager::_instance);

DECLARE(AchievementContext::getArchiveReportArray);
DECLARE(AchievementContext::instance_);

DECLARE(AchievementReporter::Ctor);
DECLARE(AchievementReporter::Dtor);
DECLARE(AchievementReporter::init);
DECLARE(AchievementReporter::update);
DECLARE(AchievementReporter::addParam);
DECLARE(AchievementReporter::finish);
DECLARE(AchievementReporter::isAllReported);
DECLARE(AchievementReporter::isReportedSecondReport);
DECLARE(AchievementReporter::eraseReported);
DECLARE(AchievementReporter::syncSaveData);

DECLARE(AchievementCheckFuncs::DecantItemTbl);

DECLARE(AchievementCheckFuncs::Ctor);
DECLARE(AchievementCheckFuncs::Dtor);
DECLARE(AchievementCheckFuncs::init);
DECLARE(AchievementCheckFuncs::clearCacheParam);
DECLARE(AchievementCheckFuncs::callFunc);
DECLARE(AchievementCheckFuncs::getDecantItemName);

DECLARE(AchievementCheckFuncs::flag);
DECLARE(AchievementCheckFuncs::flagRange);
DECLARE(AchievementCheckFuncs::hardModeBeat);
DECLARE(AchievementCheckFuncs::beatCount);
DECLARE(AchievementCheckFuncs::getItem);
DECLARE(AchievementCheckFuncs::getDecant);
DECLARE(AchievementCheckFuncs::getDecantCnt);
DECLARE(AchievementCheckFuncs::learnSummon);
DECLARE(AchievementCheckFuncs::learnSummonCnt);
DECLARE(AchievementCheckFuncs::completedMap);
DECLARE(AchievementCheckFuncs::overPlayerLevel);
DECLARE(AchievementCheckFuncs::gill);
DECLARE(AchievementCheckFuncs::monsterKillCount);
DECLARE(AchievementCheckFuncs::monsterDictionary);
DECLARE(AchievementCheckFuncs::monsterKillParty);
DECLARE(AchievementCheckFuncs::noCheck);
DECLARE(AchievementCheckFuncs::field);
DECLARE(AchievementCheckFuncs::achievementComp);

DECLARE(AchievementChecker::Ctor);
DECLARE(AchievementChecker::Dtor);
DECLARE(AchievementChecker::init);
DECLARE(AchievementChecker::update);

DECLARE(mr::getCompleteRate);

DECLARE(mon::MonsterManiaManager::monsterMania);
DECLARE(mon::MonsterManiaManager::onEncount);
DECLARE(mon::MonsterManiaManager::onLibrary);
DECLARE(mon::MonsterManiaManager::kill);
DECLARE(mon::MonsterManiaManager::clearMonsterMania);
DECLARE(mon::MonsterManiaManager::setMonsterManiaForMonsterID);

DECLARE(mon::MonsterManager::load);
DECLARE(mon::MonsterManager::free);
DECLARE(mon::MonsterManager::monsterParameter);
DECLARE(mon::MonsterManager::dropItem);
DECLARE(mon::MonsterManager::ai);
DECLARE(mon::MonsterManager::counter);
DECLARE(mon::MonsterManager::turnAction);
DECLARE(mon::MonsterManager::actionCondition);
DECLARE(mon::MonsterManager::offset);
DECLARE(mon::MonsterManager::normalAttack);
DECLARE(mon::MonsterManager::octmanmosLegInfo);
DECLARE(mon::MonsterManager::effectsInfo);
DECLARE(mon::MonsterManager::acceptMonsterManiaData);
DECLARE(mon::MonsterManager::monsterManiaManager);
DECLARE(mon::MonsterManager::sendMonsterManiaData);
DECLARE(mon::MonsterManager::setMonsterIdForMonsterManaia);
DECLARE(mon::MonsterManager::instance_);

DECLARE(mr::MonsterRefDataManager::Ctor);
DECLARE(mr::MonsterRefDataManager::Dtor);
DECLARE(mr::MonsterRefDataManager::load);
DECLARE(mr::MonsterRefDataManager::free);
DECLARE(mr::MonsterRefDataManager::monsterReference);
DECLARE(mr::MonsterRefDataManager::monsterReferenceFromIndex);
DECLARE(mr::MRDMng);

DECLARE(itm::ItemManager::consumptionParameter);
DECLARE(itm::ItemManager::weaponParameter);
DECLARE(itm::ItemManager::protectionParameter);
DECLARE(itm::ItemManager::importantParameter);
DECLARE(itm::ItemManager::equipParameter);
DECLARE(itm::ItemManager::itemParameter);
DECLARE(itm::ItemManager::itemCategory);
DECLARE(itm::ItemManager::load);
DECLARE(itm::ItemManager::free);
DECLARE(itm::ItemManager::instance_);
DECLARE(itm::PossessionItemManager::normalItem);
DECLARE(itm::PossessionItemManager::itemAllNum);
DECLARE(itm::PossessionItemManager::resetItemId);
DECLARE(itm::PossessionItemManager::initialize);
DECLARE(itm::PossessionItemManager::importantItem);
DECLARE(itm::PossessionItemManager::storeItem);
DECLARE(itm::PossessionItemManager::searchNormalItem);
DECLARE(itm::PossessionItemManager::normalItemNum);
DECLARE(itm::PossessionItemManager::importantItemNum);
DECLARE(itm::PossessionItemManager::eraseAllNormalItem);
DECLARE(itm::PossessionItemManager::reserveCancel);
DECLARE(itm::PossessionItemManager::resetImportantItemId);
DECLARE(itm::PossessionItemManager::swapItem);
DECLARE(itm::PossessionItemManager::eraseItem);
DECLARE(itm::PossessionItemManager::eraseAllImportantItem);
DECLARE(itm::PossessionItemManager::storeImportantItem);
DECLARE(itm::PossessionItemManager::searchImportantItem);

DECLARE(common::EfficacyDataConvection::Ctor);
DECLARE(common::EfficacyDataConvection::Dtor);
DECLARE(common::EfficacyDataConvection::initialize);
DECLARE(common::EfficacyDataConvection::loadBELD);
DECLARE(common::EfficacyDataConvection::unloadBELD);
DECLARE(common::EfficacyDataConvection::terminate);
DECLARE(common::EfficacyDataConvection::getEfficacyData);
DECLARE(common::EfficacyDataConvection::instance_);

DECLARE(pl::LearningAbilityManager::initialize);
DECLARE(pl::LearningAbilityManager::setContents);
DECLARE(pl::LearningAbilityManager::on);
DECLARE(pl::LearningAbilityManager::off);
DECLARE(pl::LearningAbilityManager::isOn);
DECLARE(pl::LearningAbilityManager::clear);
DECLARE(pl::LearningAbilityManager::isMagicTypesAbility);
DECLARE(pl::LearningAbilityManager::learningMagicTypesNumber);
DECLARE(pl::LearningAbilityManager::learn);
DECLARE(pl::LearningAbilityManager::learnForDebug);
DECLARE(pl::LearningAbilityManager::forget);
DECLARE(pl::LearningAbilityManager::isLearning);
DECLARE(pl::LearningAbilityManager::FLAG_SIZE);
DECLARE(pl::LearningAbilityManager::ARRAY_SIZE);
DECLARE(pl::LearningAbilityManager::FLAG_OFFSET);

DECLARE(ds::sys3d::CModelTexture::Ctor);
DECLARE(ds::sys3d::CModelTexture::Dtor);
DECLARE(ds::sys3d::CModelTexture::clearValue);
DECLARE(ds::sys3d::CModelTexture::setup);
DECLARE(ds::sys3d::CModelTexture::cleanup);
DECLARE(ds::sys3d::CModelTexture::releaseResource);
DECLARE(ds::sys3d::CModelTexture::reqReleaseResource);
DECLARE(ds::sys3d::CModelTexture::tdlhCompletion);
DECLARE(ds::sys3d::CModelTexture::isModelTexture);
DECLARE(ds::sys3d::CModelTexture::bindMdl);
DECLARE(ds::sys3d::CModelTexture::releaseMdl);
DECLARE(ds::sys3d::CModelTexture::bindMdlSet);
DECLARE(ds::sys3d::CModelTexture::releaseMdlSet);
DECLARE(ds::sys3d::CModelTexture::bindMdlToTex);
DECLARE(ds::sys3d::CModelTexture::releaseMdlToTex);
DECLARE(ds::sys3d::CModelTexture::bindMdlToTexByName);
DECLARE(ds::sys3d::CModelTexture::releaseMdlToTexByName);
DECLARE(ds::sys3d::CModelTexture::bindMdlToPltt);
DECLARE(ds::sys3d::CModelTexture::releaseMdlToPltt);
DECLARE(ds::sys3d::CModelTexture::bindMdlToPlttByName);
DECLARE(ds::sys3d::CModelTexture::releaseMdlToPlttByName);
DECLARE(ds::sys3d::CModelTexture::bindTexPlt);

DECLARE(CTextureDataMng::init);
DECLARE(CTextureDataMng::end);
DECLARE(CTextureDataMng::initValue);
DECLARE(CTextureDataMng::setTextureData);
DECLARE(CTextureDataMng::setData);
DECLARE(CTextureDataMng::setData2);
DECLARE(CTextureDataMng::searchDataIndex);
DECLARE(CTextureDataMng::searchNullIndex);
DECLARE(CTextureDataMng::useTexture);
DECLARE(CTextureDataMng::delData);
DECLARE(CTextureDataMng::getTex);
DECLARE(CTextureDataMng::setupLnkTexVram);
DECLARE(CTextureDataMng::releaseLnkTexVram);

DECLARE(itm::EquipParameter::canEquip);

DECLARE(btl::BaseBattleCharacter::isInvincible);

DECLARE(btl::BattleDebugParameter::flag);

DECLARE(btl::CBattleDisplay::doShakeCamera);

DECLARE(btl::BattleWin::possessGoldPhase);

DECLARE(btl::AcquiredGoldDrawer::update);

DECLARE(debug::CharaViewerPart::doInitialize);

DECLARE(ds::CVram::setupTexVramMng);
DECLARE(ds::CVram::setupPlttVramMng);
DECLARE(ds::CVram::releaseTexVramMng);
DECLARE(ds::CVram::releasePlttVramMng);
DECLARE(ds::CVram::m_instance);

DECLARE(evt::EventCamera::startCameraMotion);

DECLARE(ds::sys3d::CCamera::setFOV);

DECLARE(ds::snd::dssndUnloadSE1);
DECLARE(ds::snd::dssndUnloadSE2);
DECLARE(ds::snd::dssndUnloadSEAll);

DECLARE(ds::snd::dssndUnloadBGM1);
DECLARE(ds::snd::dssndUnloadBGM2);
DECLARE(ds::snd::dssndUnloadBGMAll);

void init() {
    GET_FUNCTION(initApp, "_Z7initAppP7_JNIEnv");
    GET_FUNCTION(render, "render");
    GET_FUNCTION(touch, "touch");

    GET_VARIABLE(fontScale, "fontScale");

    GET_FUNCTION(OSi_Panic, "_Z9OSi_PanicPKciS0_z");
    GET_FUNCTION(OS_Printf, "_Z9OS_PrintfPKcz");

    GET_FUNCTION(FX_Div, "_Z6FX_Divii");
    GET_FUNCTION(NNS_G2dTickCellAnimation, "_Z24NNS_G2dTickCellAnimationP19NNSG2dCellAnimationi");
    GET_FUNCTION(NNS_SndPlayerStopSeqBySeqArcIdx, "_Z31NNS_SndPlayerStopSeqBySeqArcIdxiii");

    GET_FUNCTION(GX_SetWideMode, "_Z14GX_SetWideModeib");
    GET_FUNCTION(G2_SetWnd0Position, "_Z18G2_SetWnd0Positioniiii");

    GET_FUNCTION(babilCommand_ClearCountJump, "_Z27babilCommand_ClearCountJumpR12ScriptEngine");
    GET_FUNCTION(babilCommand_CE_CameraPos, "_Z25babilCommand_CE_CameraPosR12ScriptEngine");
    GET_FUNCTION(babilCommand_CE_SetupCameraMotion, "_Z33babilCommand_CE_SetupCameraMotionR12ScriptEngine");
    GET_FUNCTION(ScriptEngine::getByte, "_ZN12ScriptEngine7getByteEv");
    GET_FUNCTION(ScriptEngine::getDword, "_ZN12ScriptEngine8getDwordEv");
    GET_FUNCTION(ScriptEngine::jump, "_ZN12ScriptEngine4jumpEj");

    GET_VARIABLE(LCD_WIDTH, "LCD_WIDTH");
    GET_VARIABLE(LCD_HEIGHT, "LCD_HEIGHT");

    GET_FUNCTION(sys::GGlobal::setNextPart, "_ZN3sys7GGlobal11setNextPartEN4part8GAMEPARTE");
    GET_VARIABLE(sys::GGlobal::getCurrentPart, "_ZN3sys7GGlobal14getCurrentPartEv");
    GET_FUNCTION(sys::GGlobal::setNextPart, "_ZN3sys7GGlobal11setNextPartEN4part8GAMEPARTE");

    GET_FUNCTION(sys::GameParameter::isDash, "_ZN3sys13GameParameter6isDashEv");
    GET_FUNCTION(sys::GameParameter::fieldSymbolID, "_ZNK3sys13GameParameter13fieldSymbolIDEv");
    GET_FUNCTION(sys::GameParameter::setFieldSymbolID, "_ZN3sys13GameParameter16setFieldSymbolIDEi");
    GET_FUNCTION(sys::GameParameter::gameClearCount, "_ZN3sys13GameParameter14gameClearCountEv");
    GET_FUNCTION(sys::GameParameter::gold, "_ZN3sys13GameParameter4goldEv");
    GET_VARIABLE(sys::GameParameter::gpInstance_, "_ZN3sys13GameParameter11gpInstance_E");
    GET_FUNCTION(sys::GameParameter::Ctor, "_ZN3sys13GameParameterC2Ev");
    GET_FUNCTION(sys::GameParameter::gpInitialize, "_ZN3sys13GameParameter12gpInitializeEv");
    GET_FUNCTION(sys::GameParameter::gpLoadDataReflect, "_ZN3sys13GameParameter17gpLoadDataReflectEv");
    GET_FUNCTION(sys::GameParameter::previousSaveData, "_ZN3sys13GameParameter16previousSaveDataEv");
    GET_FUNCTION(sys::GameParameter::gpCopy, "_ZN3sys13GameParameter6gpCopyEPv");
    GET_FUNCTION(sys::GameParameter::print, "_ZN3sys13GameParameter5printEv");
    GET_FUNCTION(sys::GameParameter::printMemory, "_ZN3sys13GameParameter11printMemoryEv");
    GET_FUNCTION(sys::GameParameter::getSaveNo, "_ZN3sys13GameParameter9getSaveNoEv");
    GET_FUNCTION(sys::GameParameter::setSaveNo, "_ZN3sys13GameParameter9setSaveNoEh");
    GET_FUNCTION(sys::GameParameter::getSaveNum, "_ZN3sys13GameParameter10getSaveNumEv");
    GET_FUNCTION(sys::GameParameter::setSaveNum, "_ZN3sys13GameParameter10setSaveNumEii");
    GET_FUNCTION(sys::GameParameter::playTime, "_ZN3sys13GameParameter8playTimeEv");
    GET_FUNCTION(sys::GameParameter::setPlayTime, "_ZN3sys13GameParameter11setPlayTimeEj");
    GET_FUNCTION(sys::GameParameter::prevSaveForTime, "_ZN3sys13GameParameter15prevSaveForTimeEv");
    GET_FUNCTION(sys::GameParameter::reflectForTime, "_ZN3sys13GameParameter14reflectForTimeEv");
    GET_FUNCTION(sys::GameParameter::prevSaveForFlag, "_ZN3sys13GameParameter15prevSaveForFlagEv");
    GET_FUNCTION(sys::GameParameter::reflectForFlag, "_ZN3sys13GameParameter14reflectForFlagEv");
    GET_FUNCTION(sys::GameParameter::prevSaveForNavimap, "_ZN3sys13GameParameter18prevSaveForNavimapEv");
    GET_FUNCTION(sys::GameParameter::prevSaveForNavimapAsync, "_ZN3sys13GameParameter23prevSaveForNavimapAsyncEv");
    GET_FUNCTION(sys::GameParameter::reflectForNavimap2, "_ZN3sys13GameParameter18reflectForNavimap2Ev");
    GET_FUNCTION(sys::GameParameter::setPartsHeaderStr, "_ZN3sys13GameParameter17setPartsHeaderStrEv");
    GET_FUNCTION(sys::GameParameter::setDash, "_ZN3sys13GameParameter7setDashEb");
    GET_FUNCTION(sys::GameParameter::isCloudGuideShowed, "_ZNK3sys13GameParameter18isCloudGuideShowedEv");
    GET_FUNCTION(sys::GameParameter::setCloudGuideShowed, "_ZN3sys13GameParameter19setCloudGuideShowedEb");
    GET_FUNCTION(sys::GameParameter::setResetCloudGuideShowed, "_ZN3sys13GameParameter24setResetCloudGuideShowedEv");
    GET_FUNCTION(sys::GameParameter::isShowConteEventTelop, "_ZNK3sys13GameParameter21isShowConteEventTelopEv");
    GET_FUNCTION(sys::GameParameter::isAutoMode, "_ZN3sys13GameParameter10isAutoModeEv");
    GET_FUNCTION(sys::GameParameter::setAutoMode, "_ZN3sys13GameParameter11setAutoModeEb");
    GET_FUNCTION(sys::GameParameter::isTVMode, "_ZN3sys13GameParameter8isTVModeEv");
    GET_FUNCTION(sys::GameParameter::setTVMode, "_ZN3sys13GameParameter9setTVModeEi");
    GET_FUNCTION(sys::GameParameter::getTVMode, "_ZN3sys13GameParameter9getTVModeEv");
    GET_FUNCTION(sys::GameParameter::isPlayVoice, "_ZNK3sys13GameParameter11isPlayVoiceEv");
    GET_FUNCTION(sys::GameParameter::incrementGameClearCount, "_ZN3sys13GameParameter23incrementGameClearCountEv");
    GET_FUNCTION(sys::GameParameter::coverNum, "_ZNK3sys13GameParameter8coverNumEv");
    GET_FUNCTION(sys::GameParameter::incrementCoverNum, "_ZN3sys13GameParameter17incrementCoverNumEv");
    GET_FUNCTION(sys::GameParameter::formation, "_ZN3sys13GameParameter9formationEv");
    GET_FUNCTION(sys::GameParameter::setFormation, "_ZN3sys13GameParameter12setFormationEN2pl21PARTY_FORMATION_TYPESE");
    GET_FUNCTION(sys::GameParameter::changeFormation, "_ZN3sys13GameParameter15changeFormationEv");
    GET_FUNCTION(sys::GameParameter::item, "_ZN3sys13GameParameter4itemEv");
    GET_FUNCTION(sys::GameParameter::rrc, "_ZN3sys13GameParameter3rrcEv");
    GET_FUNCTION(sys::GameParameter::abilityName, "_ZN3sys13GameParameter11abilityNameEv");
    GET_FUNCTION(sys::GameParameter::playerName, "_ZN3sys13GameParameter10playerNameEv");
    GET_FUNCTION(sys::GameParameter::getMapName, "_ZN3sys13GameParameter10getMapNameEv");
    GET_FUNCTION(sys::GameParameter::setMapName, "_ZN3sys13GameParameter10setMapNameEPKc");
    GET_FUNCTION(sys::GameParameter::musicPlayerMode, "_ZN3sys13GameParameter15musicPlayerModeEv");
    GET_FUNCTION(sys::GameParameter::monsterManiaManager, "_ZN3sys13GameParameter19monsterManiaManagerEv");
    GET_FUNCTION(sys::GameParameter::setMonsterManiaManager, "_ZN3sys13GameParameter22setMonsterManiaManagerERN3mon19MonsterManiaManagerE");
    GET_FUNCTION(sys::GameParameter::playerSaveParameter, "_ZN3sys13GameParameter19playerSaveParameterEN2pl12PLAYER_TYPESE");
    GET_FUNCTION(sys::GameParameter::summonSaveParameter, "_ZN3sys13GameParameter19summonSaveParameterEv");
    GET_FUNCTION(sys::GameParameter::getWorldPlayerPosition, "_ZN3sys13GameParameter22getWorldPlayerPositionEv");
    GET_FUNCTION(sys::GameParameter::setWorldPlayerPosition, "_ZN3sys13GameParameter22setWorldPlayerPositionE7VecFx32");
    GET_FUNCTION(sys::GameParameter::getWorldPlayerRotation, "_ZN3sys13GameParameter22getWorldPlayerRotationERiS1_S1_");
    GET_FUNCTION(sys::GameParameter::setWorldPlayerRotation, "_ZN3sys13GameParameter22setWorldPlayerRotationEiii");
    GET_FUNCTION(sys::GameParameter::getWorldVehiclePosition, "_ZN3sys13GameParameter23getWorldVehiclePositionEi");
    GET_FUNCTION(sys::GameParameter::setWorldVehiclePosition, "_ZN3sys13GameParameter23setWorldVehiclePositionEi7VecFx32");
    GET_FUNCTION(sys::GameParameter::getWorldVehicleRotation, "_ZN3sys13GameParameter23getWorldVehicleRotationEiRiS1_S1_");
    GET_FUNCTION(sys::GameParameter::setWorldVehicleRotation, "_ZN3sys13GameParameter23setWorldVehicleRotationEiiii");
    GET_FUNCTION(sys::GameParameter::getWorldVehicleStay, "_ZN3sys13GameParameter19getWorldVehicleStayEi");
    GET_FUNCTION(sys::GameParameter::setWorldVehicleStay, "_ZN3sys13GameParameter19setWorldVehicleStayEih");
    GET_FUNCTION(sys::GameParameter::getWorldTelepoPosition, "_ZN3sys13GameParameter22getWorldTelepoPositionEv");
    GET_FUNCTION(sys::GameParameter::setWorldTelepoPosition, "_ZN3sys13GameParameter22setWorldTelepoPositionE7VecFx32");
    GET_FUNCTION(sys::GameParameter::getWorldTelepoRotation, "_ZN3sys13GameParameter22getWorldTelepoRotationERiS1_S1_");
    GET_FUNCTION(sys::GameParameter::setWorldTelepoRotation, "_ZN3sys13GameParameter22setWorldTelepoRotationEiii");
    GET_FUNCTION(sys::GameParameter::getWorldTelepoMapName, "_ZN3sys13GameParameter21getWorldTelepoMapNameEv");
    GET_FUNCTION(sys::GameParameter::setWorldTelepoMapName, "_ZN3sys13GameParameter21setWorldTelepoMapNameEPKc");
    GET_FUNCTION(sys::GameParameter::getMagishipPosOffsetOnMoonChip, "_ZN3sys13GameParameter30getMagishipPosOffsetOnMoonChipER7VecFx32");
    GET_FUNCTION(sys::GameParameter::setMagishipPosOffsetOnMoonChip, "_ZN3sys13GameParameter30setMagishipPosOffsetOnMoonChipERK7VecFx32");
    GET_FUNCTION(sys::GameParameter::getMagishipMoonChipX, "_ZN3sys13GameParameter20getMagishipMoonChipXEv");
    GET_FUNCTION(sys::GameParameter::setMagishipMoonChipX, "_ZN3sys13GameParameter20setMagishipMoonChipXEh");
    GET_FUNCTION(sys::GameParameter::getMagishipMoonChipZ, "_ZN3sys13GameParameter20getMagishipMoonChipZEv");
    GET_FUNCTION(sys::GameParameter::setMagishipMoonChipZ, "_ZN3sys13GameParameter20setMagishipMoonChipZEh");
    GET_FUNCTION(sys::GameParameter::getMagshipMoonPos, "_ZN3sys13GameParameter17getMagshipMoonPosER7VecFx32");
    GET_FUNCTION(sys::GameParameter::setMagshipMoonPos, "_ZN3sys13GameParameter17setMagshipMoonPosERK7VecFx32");
    GET_FUNCTION(sys::GameParameter::getMagshipEarthPos, "_ZN3sys13GameParameter18getMagshipEarthPosER7VecFx32");
    GET_FUNCTION(sys::GameParameter::setMagshipEarthPos, "_ZN3sys13GameParameter18setMagshipEarthPosERK7VecFx32");
    GET_FUNCTION(sys::GameParameter::getBlackChocoboHome, "_ZN3sys13GameParameter19getBlackChocoboHomeER7VecFx32");
    GET_FUNCTION(sys::GameParameter::setBlackChocoboHome, "_ZN3sys13GameParameter19setBlackChocoboHomeERK7VecFx32");
    GET_FUNCTION(sys::GameParameter::getDesionMapName, "_ZN3sys13GameParameter16getDesionMapNameEi");
    GET_FUNCTION(sys::GameParameter::setDesionMapName, "_ZN3sys13GameParameter16setDesionMapNameEiPKc");
    GET_FUNCTION(sys::GameParameter::getDesionPosition, "_ZN3sys13GameParameter17getDesionPositionEi");
    GET_FUNCTION(sys::GameParameter::setDesionPosition, "_ZN3sys13GameParameter17setDesionPositionEiR7VecFx32");
    GET_FUNCTION(sys::GameParameter::getDesionRot, "_ZN3sys13GameParameter12getDesionRotEiRsS1_S1_");
    GET_FUNCTION(sys::GameParameter::setDesionRot, "_ZN3sys13GameParameter12setDesionRotEisss");
    GET_FUNCTION(sys::GameParameter::getDesionDataValidity, "_ZN3sys13GameParameter21getDesionDataValidityEv");
    GET_FUNCTION(sys::GameParameter::setDesionDataValidity, "_ZN3sys13GameParameter21setDesionDataValidityEb");
    GET_FUNCTION(sys::GameParameter::getPassagePoint2, "_ZN3sys13GameParameter16getPassagePoint2Ei");
    GET_FUNCTION(sys::GameParameter::setPassagePoint2, "_ZN3sys13GameParameter16setPassagePoint2EiPj");

    GET_VARIABLE(ui::g_WidgetMng, "_ZN2ui11g_WidgetMngE");
    GET_FUNCTION(ui::CWidgetMng::initialize, "_ZN2ui10CWidgetMng10initializeEv");
    GET_FUNCTION(ui::CWidgetMng::terminate, "_ZN2ui10CWidgetMng9terminateEv");
    GET_FUNCTION(ui::CWidgetMng::cleanWidget, "_ZN2ui10CWidgetMng11cleanWidgetEv");
    GET_FUNCTION(ui::CWidgetMng::addWidget, "_ZN2ui10CWidgetMng9addWidgetEiiiiiiii");
    GET_FUNCTION(ui::CWidgetMng::deleteWidget, "_ZN2ui10CWidgetMng12deleteWidgetEPNS_6WidgetE");
    GET_FUNCTION(ui::CWidgetMng::deleteWidgetRange, "_ZN2ui10CWidgetMng12deleteWidgetEii");
    GET_FUNCTION(ui::CWidgetMng::findWidget, "_ZN2ui10CWidgetMng10findWidgetEi");
    GET_FUNCTION(ui::CWidgetMng::draw, "_ZN2ui10CWidgetMng4drawEv");
    GET_FUNCTION(ui::CWidgetMng::drawWidget, "_ZN2ui10CWidgetMng10drawWidgetEv");
    GET_FUNCTION(ui::CWidgetMng::execute, "_ZN2ui10CWidgetMng7executeEv");
    GET_FUNCTION(ui::CWidgetMng::moveWidget, "_ZN2ui10CWidgetMng10moveWidgetEv");
    GET_FUNCTION(ui::CWidgetMng::moveSlide, "_ZN2ui10CWidgetMng9moveSlideEv");
    GET_FUNCTION(ui::CWidgetMng::setWidgetHilight, "_ZN2ui10CWidgetMng16setWidgetHilightEiii");
    GET_FUNCTION(ui::CWidgetMng::setWidgetEnable, "_ZN2ui10CWidgetMng15setWidgetEnableEiic");
    GET_FUNCTION(ui::CWidgetMng::getActiveCursor, "_ZN2ui10CWidgetMng15getActiveCursorEv");
    GET_FUNCTION(ui::CWidgetMng::getActiveCursorItem, "_ZN2ui10CWidgetMng19getActiveCursorItemEv");
    GET_FUNCTION(ui::CWidgetMng::getActiveCursorListMax, "_ZN2ui10CWidgetMng22getActiveCursorListMaxEv");
    GET_FUNCTION(ui::CWidgetMng::getCursorItem, "_ZN2ui10CWidgetMng13getCursorItemEi");
    GET_FUNCTION(ui::CWidgetMng::setCursor, "_ZN2ui10CWidgetMng9setCursorEiiiib");
    GET_FUNCTION(ui::CWidgetMng::setListMax, "_ZN2ui10CWidgetMng10setListMaxEii");
    GET_FUNCTION(ui::CWidgetMng::getListMax, "_ZN2ui10CWidgetMng10getListMaxEi");
    GET_FUNCTION(ui::CWidgetMng::setScroll, "_ZN2ui10CWidgetMng9setScrollEiii");
    GET_FUNCTION(ui::CWidgetMng::setScrollPos, "_ZN2ui10CWidgetMng12setScrollPosEi");
    GET_FUNCTION(ui::CWidgetMng::setScrollRect, "_ZN2ui10CWidgetMng13setScrollRectEiiiii");
    GET_FUNCTION(ui::CWidgetMng::setSlide, "_ZN2ui10CWidgetMng8setSlideEii");
    GET_FUNCTION(ui::CWidgetMng::setDefaultPadKeyFunc, "_ZN2ui10CWidgetMng20setDefaultPadKeyFuncEv");
    GET_FUNCTION(ui::CWidgetMng::setDragState, "_ZN2ui10CWidgetMng12setDragStateEb");
    GET_FUNCTION(ui::CWidgetMng::DefaultPadKeyFunc, "_ZN2ui10CWidgetMng17DefaultPadKeyFuncEib");
    GET_FUNCTION(ui::CWidgetMng::DefaultPadKeyFunc_Line2, "_ZN2ui10CWidgetMng23DefaultPadKeyFunc_Line2Eib");
    GET_FUNCTION(ui::CWidgetMng::DefaultPadKeyFunc_Line3, "_ZN2ui10CWidgetMng23DefaultPadKeyFunc_Line3Eib");
    GET_FUNCTION(ui::CWidgetMng::DefaultPadKeyFunc_LineMulti, "_ZN2ui10CWidgetMng27DefaultPadKeyFunc_LineMultiEiib");

    GET_FUNCTION(world::MSSConfig::mssInitialize, "_ZN5world9MSSConfig13mssInitializeEv");
    GET_FUNCTION(world::MSSConfig::mssProcess, "_ZN5world9MSSConfig10mssProcessEv");

    GET_FUNCTION(world::enableSPSetting, "_ZN5world15enableSPSettingEPNS_17WorldStateContextE");
    GET_FUNCTION(world::disableSPSetting, "_ZN5world16disableSPSettingEPNS_17WorldStateContextE");
    GET_FUNCTION(world::searchNextSymbolByCharacterIDBackward, "_ZN5world37searchNextSymbolByCharacterIDBackwardEi");
    GET_FUNCTION(world::searchNextSymbolByCharacterIDForward, "_ZN5world36searchNextSymbolByCharacterIDForwardEi");
    GET_FUNCTION(world::canChangeSymbol, "_ZN5world15canChangeSymbolEv");
    GET_FUNCTION(world::MSSTextScreenClear, "_ZN5world18MSSTextScreenClearEN3msg6CANVASE");

    GET_FUNCTION(world::WSMove::WSMove, "_ZN5world6WSMoveC1Ev");
    GET_FUNCTION(world::WSMove::wsmEncount, "_ZN5world6WSMove10wsmEncountEPNS_17WorldStateContextE");
    GET_FUNCTION(world::WSMove::wsIsMenuOpen, "_ZN5world6WSMove12wsIsMenuOpenEPNS_17WorldStateContextE");

    GET_FUNCTION(world::WorldState::wsGetString, "_ZN5world10WorldState11wsGetStringEv");
    GET_FUNCTION(world::WorldState::wsSetEnd, "_ZN5world10WorldState8wsSetEndEb");

    GET_FUNCTION(world::WorldTask::queryTask, "_ZN5world9WorldTask9queryTaskEPKc");
    GET_FUNCTION(world::WorldTask::deleteTask, "_ZN5world9WorldTask10deleteTaskEPKc");

    GET_FUNCTION(world::WorldStateScheduler::wssInitialize, "_ZN5world19WorldStateScheduler13wssInitializeEv");
    GET_FUNCTION(world::WorldStateScheduler::wssAddStateSchedule, "_ZN5world19WorldStateScheduler19wssAddStateScheduleEPKc");
    GET_FUNCTION(world::WorldStateScheduler::wssGetCurrentState, "_ZNK5world19WorldStateScheduler18wssGetCurrentStateEv");

    GET_FUNCTION(world::WSVehicleMove::wsProcess, "_ZN5world13WSVehicleMove9wsProcessEPNS_17WorldStateContextE");

    GET_FUNCTION(world::CCameraVibration::startVibration, "_ZN5world16CCameraVibration14startVibrationENS0_15VIBRATION_STATEEiiiiib");
    GET_FUNCTION(world::CCameraVibration::ccbUpdate, "_ZN5world16CCameraVibration9ccbUpdateER7VecFx32S2_");

    GET_FUNCTION(btl::BattlePart::doInitialize, "_ZN3btl10BattlePart12doInitializeEv");
    GET_VARIABLE(btl::BattlePart::instance_, "_ZN3btl10BattlePart9instance_E");

    GET_FUNCTION(btl::battleSpeedRate, "_ZN3btl15battleSpeedRateEv");

    GET_FUNCTION(btl::Battle2DManager::ctrlWidgetCheck, "_ZN3btl15Battle2DManager15ctrlWidgetCheckENS0_16WIDGET_CTRL_TYPEE");
    GET_VARIABLE(btl::Battle2DManager::instance_, "_ZN3btl15Battle2DManager9instance_E");

    GET_FUNCTION(btl::BattleMonsterParty::giftGold, "_ZN3btl18BattleMonsterParty8giftGoldEv");
    GET_FUNCTION(btl::BattleMonsterParty::giftExp, "_ZN3btl18BattleMonsterParty7giftExpEv");

    GET_FUNCTION(btl::BattlePlayer::playerId, "_ZN3btl12BattlePlayer8playerIdEv");

    GET_FUNCTION(btl::BattleCommandSelector::initialize, "_ZN3btl21BattleCommandSelector10initializeERNS_12BattleSystemEPNS_12BattlePlayerE");
    GET_FUNCTION(btl::BattleCommandSelector::commandAction, "_ZN3btl21BattleCommandSelector13commandActionERNS_12BattleSystemEN6common10ABILITY_IDEc");

    GET_FUNCTION(btl::CBattleDisplay::doShakeCamera, "_ZN3btl14CBattleDisplay13doShakeCameraEv");

    GET_FUNCTION(btl::BattleWin::possessGoldPhase, "_ZN3btl9BattleWin16possessGoldPhaseERNS_12BattleSystemE");

    GET_FUNCTION(btl::AcquiredGoldDrawer::update, "_ZN3btl18AcquiredGoldDrawer6updateEv");

    GET_FUNCTION(CCharacterMng::getIndexOfSearchingCharacter, "_ZN13CCharacterMng28getIndexOfSearchingCharacterEv");
    GET_FUNCTION(CCharacterMng::getCharacterName, "_ZN13CCharacterMng16getCharacterNameEi");
    GET_FUNCTION(CCharacterMng::setHidden, "_ZN13CCharacterMng9setHiddenEib");
    GET_FUNCTION(CCharacterMng::setShadowEnable, "_ZN13CCharacterMng15setShadowEnableEib");
    GET_FUNCTION(CCharacterMng::setShadowType, "_ZN13CCharacterMng13setShadowTypeEii");
    GET_FUNCTION(CCharacterMng::setShadowAlphaRate, "_ZN13CCharacterMng18setShadowAlphaRateEii");
    GET_FUNCTION(CCharacterMng::setTransparencyRate, "_ZN13CCharacterMng19setTransparencyRateEii");
    GET_FUNCTION(CCharacterMng::setPosition, "_ZN13CCharacterMng11setPositionEiRK7VecFx32");
    GET_FUNCTION(CCharacterMng::setRotation, "_ZN13CCharacterMng11setRotationEittt");
    GET_FUNCTION(CCharacterMng::startMotion, "_ZN13CCharacterMng11startMotionEiibj");
    GET_FUNCTION(CCharacterMng::setPause, "_ZN13CCharacterMng8setPauseEibN2ds5sys3d8CAnimSet6enTYPEE");
    GET_FUNCTION(CCharacterMng::setFrameRate, "_ZN13CCharacterMng12setFrameRateEi");
    GET_VARIABLE(characterMng, "characterMng");

    GET_FUNCTION(ys::Condition::isNotBattleCondition, "_ZN2ys9Condition20isNotBattleConditionEv");

    GET_FUNCTION(pl::Player::condition, "_ZN2pl6Player9conditionEv");
    GET_FUNCTION(pl::Player::Ctor, "_ZN2pl6PlayerC2Ev");
    GET_FUNCTION(pl::Player::Dtor, "_ZN2pl6PlayerD2Ev");
    GET_FUNCTION(pl::Player::initialize, "_ZN2pl6Player10initializeENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::Player::initializeName, "_ZN2pl6Player14initializeNameEv");
    GET_FUNCTION(pl::Player::initializeCommand, "_ZN2pl6Player17initializeCommandEv");
    GET_FUNCTION(pl::Player::initializeDefaultCommand, "_ZN2pl6Player24initializeDefaultCommandEv");
    GET_FUNCTION(pl::Player::updateParameter, "_ZN2pl6Player15updateParameterEv");
    GET_FUNCTION(pl::Player::updateCondition, "_ZN2pl6Player15updateConditionEv");
    GET_FUNCTION(pl::Player::recoverHPandMP, "_ZN2pl6Player14recoverHPandMPEv");
    GET_FUNCTION(pl::Player::useApple, "_ZN2pl6Player8useAppleEb");
    GET_FUNCTION(pl::Player::useSomaDrop, "_ZN2pl6Player11useSomaDropEv");
    GET_FUNCTION(pl::Player::fine, "_ZN2pl6Player4fineEv");
    GET_FUNCTION(pl::Player::playerAbilityManager, "_ZN2pl6Player20playerAbilityManagerEv");
    GET_FUNCTION(pl::Player::learningAbility, "_ZN2pl6Player15learningAbilityEv");
    GET_FUNCTION(pl::Player::equipParameter, "_ZN2pl6Player14equipParameterEv");
    GET_FUNCTION(pl::Player::bodyAndBonus, "_ZN2pl6Player12bodyAndBonusEv");
    GET_FUNCTION(pl::Player::level, "_ZN2pl6Player5levelEv");
    GET_FUNCTION(pl::Player::hp, "_ZN2pl6Player2hpEv");
    GET_FUNCTION(pl::Player::mp, "_ZN2pl6Player2mpEv");
    GET_FUNCTION(pl::Player::exp, "_ZN2pl6Player3expEv");
    GET_FUNCTION(pl::Player::decantLevel, "_ZN2pl6Player11decantLevelEv");
    GET_FUNCTION(pl::Player::saveParameter, "_ZN2pl6Player13saveParameterEv");
    GET_FUNCTION(pl::Player::growParameter, "_ZN2pl6Player13growParameterEh");
    GET_FUNCTION(pl::Player::levelUp, "_ZN2pl6Player7levelUpEi");
    GET_FUNCTION(pl::Player::levelDown, "_ZN2pl6Player9levelDownEi");
    GET_FUNCTION(pl::Player::setParameter, "_ZN2pl6Player12setParameterEhh");
    GET_FUNCTION(pl::Player::setExp, "_ZN2pl6Player6setExpEh");
    GET_FUNCTION(pl::Player::setHp, "_ZN2pl6Player5setHpEh");
    GET_FUNCTION(pl::Player::bodyAndBonus_BP, "_ZN2pl6Player12bodyAndBonusERN2ys13BodyParameterEsssss");
    GET_FUNCTION(pl::Player::calcBonus, "_ZN2pl6Player9calcBonusEv");
    GET_FUNCTION(pl::Player::equipmentBounsHP, "_ZN2pl6Player16equipmentBounsHPEv");
    GET_FUNCTION(pl::Player::abilityBouns, "_ZN2pl6Player12abilityBounsEv");
    GET_FUNCTION(pl::Player::addEquipmentBonus, "_ZN2pl6Player17addEquipmentBonusERN2ys13BodyParameterEi");
    GET_FUNCTION(pl::Player::setPhysicsAttack, "_ZN2pl6Player16setPhysicsAttackEv");
    GET_FUNCTION(pl::Player::calcPhysicsDefense, "_ZN2pl6Player18calcPhysicsDefenseEv");
    GET_FUNCTION(pl::Player::calcMagicDefense, "_ZN2pl6Player16calcMagicDefenseEv");
    GET_FUNCTION(pl::Player::magicDefensePower, "_ZN2pl6Player17magicDefensePowerEv");
    GET_FUNCTION(pl::Player::physicsAttack, "_ZN2pl6Player13physicsAttackERN2ys22PhysicsAttackParameterEsssss");
    GET_FUNCTION(pl::Player::physicsDefense, "_ZN2pl6Player14physicsDefenseERN2ys23PhysicsDefenseParameterEsssss");
    GET_FUNCTION(pl::Player::magicDefense, "_ZN2pl6Player12magicDefenseERN2ys21MagicDefenseParameterEsssss");
    GET_FUNCTION(pl::Player::addMagicDefense, "_ZN2pl6Player15addMagicDefenseERN2ys21MagicDefenseParameterEi");
    GET_FUNCTION(pl::Player::addPhysicsDefense_ib, "_ZN2pl6Player17addPhysicsDefenseERN2ys23PhysicsDefenseParameterEib");
    GET_FUNCTION(pl::Player::addPhysicsDefense_Pb, "_ZN2pl6Player17addPhysicsDefenseERN2ys23PhysicsDefenseParameterEPN3itm19ProtectionParameterEb");
    GET_FUNCTION(pl::Player::avoidance, "_ZN2pl6Player9avoidanceEv");
    GET_FUNCTION(pl::Player::deftness, "_ZN2pl6Player8deftnessEv");
    GET_FUNCTION(pl::Player::defaultMaxHp, "_ZN2pl6Player12defaultMaxHpEv");
    GET_FUNCTION(pl::Player::defaultMaxMp, "_ZN2pl6Player12defaultMaxMpEv");
    GET_FUNCTION(pl::Player::calcHpColor, "_ZN2pl6Player11calcHpColorEii");
    GET_FUNCTION(pl::Player::checkHpColor, "_ZN2pl6Player12checkHpColorEv");
    GET_FUNCTION(pl::Player::isHealing, "_ZN2pl6Player9isHealingEv");
    GET_FUNCTION(pl::Player::difference, "_ZN2pl6Player10differenceEii");
    GET_FUNCTION(pl::Player::canWinningDirection, "_ZN2pl6Player19canWinningDirectionEv");
    GET_FUNCTION(pl::Player::canGivenExp, "_ZN2pl6Player11canGivenExpEv");
    GET_FUNCTION(pl::Player::isFinishAttack, "_ZN2pl6Player14isFinishAttackEv");
    GET_FUNCTION(pl::Player::isUseMagicCharacter, "_ZN2pl6Player19isUseMagicCharacterEv");
    GET_FUNCTION(pl::Player::isCommand, "_ZN2pl6Player9isCommandEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::Player::isKnowledgeInCommand, "_ZN2pl6Player20isKnowledgeInCommandEv");
    GET_FUNCTION(pl::Player::listupCommand, "_ZN2pl6Player13listupCommandERNS_17SummonAbilityListE");
    GET_FUNCTION(pl::Player::listupCommandNormal, "_ZN2pl6Player19listupCommandNormalERNS_17SummonAbilityListE");
    GET_FUNCTION(pl::Player::listupCommandMagic, "_ZN2pl6Player18listupCommandMagicERNS_17SummonAbilityListENS1_5TYPESE");
    GET_FUNCTION(pl::Player::clearCommand, "_ZN2pl6Player12clearCommandEv");
    GET_FUNCTION(pl::Player::stonePaletteName, "_ZN2pl6Player16stonePaletteNameEPc");
    GET_FUNCTION(pl::Player::changeStonePalette, "_ZN2pl6Player18changeStonePaletteEb");
    GET_FUNCTION(pl::Player::changeStonePaletteForEvent, "_ZN2pl6Player26changeStonePaletteForEventEb");
    GET_FUNCTION(pl::Player::canEquip, "_ZN2pl6Player8canEquipEsNS_9HAND_TYPEE");
    GET_FUNCTION(pl::Player::canEquipWeapon, "_ZN2pl6Player14canEquipWeaponEiPN3itm15WeaponParameterE");
    GET_FUNCTION(pl::Player::canEquipBow, "_ZN2pl6Player11canEquipBowEiPN3itm15WeaponParameterEb");
    GET_FUNCTION(pl::Player::canEquipArrow, "_ZN2pl6Player13canEquipArrowEiPN3itm15WeaponParameterE");
    GET_FUNCTION(pl::Player::canEquipHarp, "_ZN2pl6Player12canEquipHarpEiPN3itm15WeaponParameterEb");
    GET_FUNCTION(pl::Player::isCanEquipProtection, "_ZN2pl6Player20isCanEquipProtectionEiPN3itm19ProtectionParameterE");
    GET_FUNCTION(pl::Player::isHandCheck, "_ZN2pl6Player11isHandCheckEiPN3itm15WeaponParameterEPNS1_19ProtectionParameterEbb");
    GET_FUNCTION(pl::Player::isEquipable, "_ZN2pl6Player11isEquipableENS_12EQUIP_POINTSEs");
    GET_FUNCTION(pl::Player::isEquipItem, "_ZN2pl6Player11isEquipItemEPN3itm14EquipParameterE");
    GET_FUNCTION(pl::Player::isEquippedBow, "_ZN2pl6Player13isEquippedBowEv");
    GET_FUNCTION(pl::Player::isEquipedItemOnPoint, "_ZN2pl6Player20isEquipedItemOnPointEis");
    GET_FUNCTION(pl::Player::isEquipedItemOnAllPoints, "_ZN2pl6Player24isEquipedItemOnAllPointsEs");
    GET_FUNCTION(pl::Player::isEquipMetal, "_ZN2pl6Player12isEquipMetalEv");
    GET_FUNCTION(pl::Player::isHand, "_ZN2pl6Player6isHandEib");
    GET_FUNCTION(pl::Player::doEquip, "_ZN2pl6Player7doEquipEisb");
    GET_FUNCTION(pl::Player::equipmentAttach, "_ZN2pl6Player15equipmentAttachENS_9HAND_TYPEE");
    GET_FUNCTION(pl::Player::attachEquipmentSymbol, "_ZN2pl6Player21attachEquipmentSymbolEb");
    GET_FUNCTION(pl::Player::attachEquipmentSymbolOne, "_ZN2pl6Player24attachEquipmentSymbolOneENS_9HAND_TYPEEb");
    GET_FUNCTION(pl::Player::removeEquipmentSymbol, "_ZN2pl6Player21removeEquipmentSymbolEv");
    GET_FUNCTION(pl::Player::removeEquipmentSymbolOne, "_ZN2pl6Player24removeEquipmentSymbolOneENS_9HAND_TYPEE");
    GET_FUNCTION(pl::Player::equipmentSymbolVisibility, "_ZN2pl6Player25equipmentSymbolVisibilityEb");
    GET_FUNCTION(pl::Player::updateEquipmentSymbol, "_ZN2pl6Player21updateEquipmentSymbolEv");
    GET_FUNCTION(pl::Player::releaseEquipItem, "_ZN2pl6Player16releaseEquipItemEi");
    GET_FUNCTION(pl::Player::adjustAbility, "_ZN2pl6Player13adjustAbilityEv");
    GET_FUNCTION(pl::Player::equipMenuParameter, "_ZN2pl6Player18equipMenuParameterEsssss");
    GET_FUNCTION(pl::Player::spendMp, "_ZN2pl6Player7spendMpEPN6common19BabilMagicParameterE");
    GET_FUNCTION(pl::Player::isUseMagic_BM, "_ZN2pl6Player10isUseMagicEPN6common19BabilMagicParameterEib");
    GET_FUNCTION(pl::Player::isUseMagic_AI, "_ZN2pl6Player10isUseMagicEN6common10ABILITY_IDEib");
    GET_FUNCTION(pl::Player::learnAbility_ii, "_ZN2pl6Player12learnAbilityEii");
    GET_FUNCTION(pl::Player::learnAbility_AI, "_ZN2pl6Player12learnAbilityEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::Player::forgetAbility_i, "_ZN2pl6Player13forgetAbilityEi");
    GET_FUNCTION(pl::Player::forgetAbility_AI, "_ZN2pl6Player13forgetAbilityEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::Player::forgetLearnedMagicByEvent, "_ZN2pl6Player25forgetLearnedMagicByEventEv");
    GET_FUNCTION(pl::Player::learnAbilityForDebug, "_ZN2pl6Player20learnAbilityForDebugEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::Player::learnAllAbilityForDebug, "_ZN2pl6Player23learnAllAbilityForDebugEv");
    GET_FUNCTION(pl::Player::learnAllWhiteMagicForDebug, "_ZN2pl6Player26learnAllWhiteMagicForDebugEv");
    GET_FUNCTION(pl::Player::learnAllBlackMagicForDebug, "_ZN2pl6Player26learnAllBlackMagicForDebugEv");
    GET_FUNCTION(pl::Player::learnAllSummonForDebug, "_ZN2pl6Player22learnAllSummonForDebugEv");
    GET_FUNCTION(pl::Player::learnAllSongForDebug, "_ZN2pl6Player20learnAllSongForDebugEv");
    GET_FUNCTION(pl::Player::learnAllNinjutsuForDebug, "_ZN2pl6Player24learnAllNinjutsuForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllAbilityForDebug, "_ZN2pl6Player24forgetAllAbilityForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllWhiteMagicForDebug, "_ZN2pl6Player27forgetAllWhiteMagicForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllBlackMagicForDebug, "_ZN2pl6Player27forgetAllBlackMagicForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllSummonForDebug, "_ZN2pl6Player23forgetAllSummonForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllSongForDebug, "_ZN2pl6Player21forgetAllSongForDebugEv");
    GET_FUNCTION(pl::Player::forgetAllNinjutsuForDebug, "_ZN2pl6Player25forgetAllNinjutsuForDebugEv");

    GET_FUNCTION(pl::PlayerParty::member, "_ZN2pl11PlayerParty6memberENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::memberOrder, "_ZN2pl11PlayerParty11memberOrderENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::memberForOrder, "_ZN2pl11PlayerParty14memberForOrderEh");
    GET_FUNCTION(pl::PlayerParty::player, "_ZN2pl11PlayerParty6playerENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::isJoinedParty, "_ZN2pl11PlayerParty13isJoinedPartyENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::isPartyFull, "_ZN2pl11PlayerParty11isPartyFullEv");
    GET_FUNCTION(pl::PlayerParty::isEnableMember, "_ZN2pl11PlayerParty14isEnableMemberEv");
    GET_FUNCTION(pl::PlayerParty::partyMemberEnableNumber, "_ZN2pl11PlayerParty23partyMemberEnableNumberEv");
    GET_FUNCTION(pl::PlayerParty::aliveNumber, "_ZN2pl11PlayerParty11aliveNumberEv");
    GET_FUNCTION(pl::PlayerParty::averageLevel, "_ZN2pl11PlayerParty12averageLevelENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::isFrogAll, "_ZN2pl11PlayerParty9isFrogAllEv");
    GET_FUNCTION(pl::PlayerParty::isLilliputAll, "_ZN2pl11PlayerParty13isLilliputAllEv");
    GET_FUNCTION(pl::PlayerParty::saveComposition, "_ZN2pl11PlayerParty15saveCompositionEv");
    GET_FUNCTION(pl::PlayerParty::loadComposition, "_ZN2pl11PlayerParty15loadCompositionEv");
    GET_FUNCTION(pl::PlayerParty::setMemberForOrder, "_ZN2pl11PlayerParty17setMemberForOrderENS_12PLAYER_TYPESEi");
    GET_FUNCTION(pl::PlayerParty::changeMemberForOrder, "_ZN2pl11PlayerParty20changeMemberForOrderEhh");
    GET_FUNCTION(pl::PlayerParty::changeMember, "_ZN2pl11PlayerParty12changeMemberENS_12PLAYER_TYPESES1_");
    GET_FUNCTION(pl::PlayerParty::addMember, "_ZN2pl11PlayerParty9addMemberENS_12PLAYER_TYPESEi");
    GET_FUNCTION(pl::PlayerParty::releaseMember, "_ZN2pl11PlayerParty13releaseMemberENS_12PLAYER_TYPESE");
    GET_FUNCTION(pl::PlayerParty::Ctor, "_ZN2pl11PlayerPartyC2Ev");
    GET_FUNCTION(pl::PlayerParty::Dtor, "_ZN2pl11PlayerPartyD2Ev");
    GET_FUNCTION(pl::PlayerParty::initialize, "_ZN2pl11PlayerParty10initializeEv");
    GET_FUNCTION(pl::PlayerParty::initForNewgame, "_ZN2pl11PlayerParty14initForNewgameEb");
    GET_FUNCTION(pl::PlayerParty::initializeName, "_ZN2pl11PlayerParty14initializeNameEv");
    GET_FUNCTION(pl::PlayerParty::load, "_ZN2pl11PlayerParty4loadEv");
    GET_FUNCTION(pl::PlayerParty::free, "_ZN2pl11PlayerParty4freeEv");
    GET_FUNCTION(pl::PlayerParty::fineAll, "_ZN2pl11PlayerParty7fineAllEv");
    GET_FUNCTION(pl::PlayerParty::changeStonePalette, "_ZN2pl11PlayerParty18changeStonePaletteEb");
    GET_FUNCTION(pl::PlayerParty::changeStonePaletteForEvent, "_ZN2pl11PlayerParty26changeStonePaletteForEventEb");
    GET_FUNCTION(pl::PlayerParty::clearBattleCommand, "_ZN2pl11PlayerParty18clearBattleCommandEv");
    GET_FUNCTION(pl::PlayerParty::clearBattleCommandPlayer, "_ZN2pl11PlayerParty24clearBattleCommandPlayerEi");
    GET_FUNCTION(pl::PlayerParty::setSaveStartCommand, "_ZN2pl11PlayerParty19setSaveStartCommandEii");
    GET_FUNCTION(pl::PlayerParty::isCommand, "_ZN2pl11PlayerParty9isCommandEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::PlayerParty::listupCommand, "_ZN2pl11PlayerParty13listupCommandERNS_17SummonAbilityListE");
    GET_FUNCTION(pl::PlayerParty::clearBattleCondition, "_ZN2pl11PlayerParty20clearBattleConditionEv");
    GET_FUNCTION(pl::PlayerParty::addItem, "_ZN2pl11PlayerParty7addItemEii");
    GET_FUNCTION(pl::PlayerParty::formation, "_ZN2pl11PlayerParty9formationEh");
    GET_FUNCTION(pl::PlayerParty::setFormation, "_ZN2pl11PlayerParty12setFormationENS_21PARTY_FORMATION_TYPESE");
    GET_FUNCTION(pl::PlayerParty::changeFormation, "_ZN2pl11PlayerParty15changeFormationEv");
    GET_FUNCTION(pl::PlayerParty::levelParameter, "_ZN2pl11PlayerParty14levelParameterENS_12PLAYER_TYPESEc");
    GET_FUNCTION(pl::PlayerParty::learningAbility, "_ZN2pl11PlayerParty15learningAbilityENS_12PLAYER_TYPESEi");
    GET_FUNCTION(pl::PlayerParty::isLearnByLevelUp, "_ZN2pl11PlayerParty16isLearnByLevelUpENS_12PLAYER_TYPESEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::PlayerParty::normalMagic, "_ZN2pl11PlayerParty11normalMagicEi");
    GET_FUNCTION(pl::PlayerParty::normalAttack, "_ZN2pl11PlayerParty12normalAttackEi");
    GET_FUNCTION(pl::PlayerParty::abilityList, "_ZN2pl11PlayerParty11abilityListEi");
    GET_FUNCTION(pl::PlayerParty::layoutSceneParameter, "_ZN2pl11PlayerParty20layoutSceneParameterEi");
    GET_FUNCTION(pl::PlayerParty::weaponOffsetParameter, "_ZN2pl11PlayerParty21weaponOffsetParameterEN3itm13WEAPON_SYSTEME");
    GET_FUNCTION(pl::PlayerParty::abilityBouns, "_ZN2pl11PlayerParty12abilityBounsEN6common10ABILITY_IDE");
    GET_VARIABLE(pl::PlayerParty::playerPartyInstance_, "_ZN2pl11PlayerParty20playerPartyInstance_E");
    GET_VARIABLE(pl::PlayerParty::InvalidPlayer, "_ZN2pl11PlayerParty13InvalidPlayerE");

    GET_FUNCTION(map2d::NaviMap::MapOn, "_ZN5map2d7NaviMap5MapOnEv");
    GET_FUNCTION(map2d::NaviMap::MapOff, "_ZN5map2d7NaviMap6MapOffEv");
    GET_FUNCTION(map2d::NaviMap::setAlpha, "_ZN5map2d7NaviMap8setAlphaEi");
    GET_FUNCTION(map2d::NaviMap::NaviMapDrawOff, "_ZN5map2d7NaviMap14NaviMapDrawOffEv");
    GET_VARIABLE(map2d::NAVI_MAP_TASK_NAME, "_ZN5map2d18NAVI_MAP_TASK_NAMEE");

    GET_VARIABLE(part::WorldPart::wpInstance_, "_ZN4part9WorldPart11wpInstance_E");

    GET_FUNCTION(menu::playSECommon, "_ZN4menu12playSECommonEii");

    GET_FUNCTION(CoreAudioOutInit, "CoreAudioOutInit");
    GET_FUNCTION(CoreAudioOutKick, "CoreAudioOutKick");
    GET_FUNCTION(CoreAudioOutQueueBuffer, "CoreAudioOutQueueBuffer");
    GET_FUNCTION(CoreAudioOutRelease, "CoreAudioOutRelease");
    GET_FUNCTION(CoreAudioOutResume, "CoreAudioOutResume");
    GET_FUNCTION(CoreAudioOutSuspend, "CoreAudioOutSuspend");
    GET_FUNCTION(NNS_SndUpdate, "_Z13NNS_SndUpdatev");
    GET_FUNCTION(NNS_SndPlayerSetPlayerVolume, "_Z28NNS_SndPlayerSetPlayerVolumeii");

    GET_FUNCTION(ds::snd::BGMHandle::Ctor, "_ZN2ds3snd9BGMHandleC1Ev");
    GET_FUNCTION(ds::snd::BGMHandle::Dtor, "_ZN2ds3snd9BGMHandleD1Ev");
    GET_FUNCTION(ds::snd::BGMHandle::IsPlaying, "_ZN2ds3snd9BGMHandle9IsPlayingEv");
    GET_FUNCTION(ds::snd::BGMHandle::PlayV, "_ZN2ds3snd9BGMHandle4PlayERKi");
    GET_FUNCTION(ds::snd::BGMHandle::Play, "_ZN2ds3snd9BGMHandle4PlayEiiii");
    GET_FUNCTION(ds::snd::BGMHandle::SetVolume, "_ZN2ds3snd9BGMHandle9SetVolumeEi");
    GET_FUNCTION(ds::snd::BGMHandle::MoveVolume, "_ZN2ds3snd9BGMHandle10MoveVolumeEii");
    GET_FUNCTION(ds::snd::BGMHandle::Stop, "_ZN2ds3snd9BGMHandle4StopEi");
    GET_FUNCTION(ds::snd::BGMHandle::seqNo, "_ZN2ds3snd9BGMHandle5seqNoEv");
    GET_FUNCTION(ds::snd::BGMHandle::Pause, "_ZN2ds3snd9BGMHandle5PauseEc");
    GET_FUNCTION(ds::snd::BGMHandle::setBGMEnable, "_ZN2ds3snd9BGMHandle12setBGMEnableEb");
    GET_FUNCTION(ds::snd::BGMHandle::getBGMEnable, "_ZN2ds3snd9BGMHandle12setBGMEnableEv");
    GET_FUNCTION(ds::snd::BGMHandle::forceReleaseSequence, "_ZN2ds3snd9BGMHandle20forceReleaseSequenceEv");
    GET_FUNCTION(ds::snd::BGMHandle::setTempo, "_ZN2ds3snd9BGMHandle8setTempoEi");

    GET_FUNCTION(ds::snd::SEHandle::Ctor, "_ZN2ds3snd8SEHandleC1Ev");
    GET_FUNCTION(ds::snd::SEHandle::Dtor, "_ZN2ds3snd8SEHandleD1Ev");
    GET_FUNCTION(ds::snd::SEHandle::IsPlaying, "_ZN2ds3snd8SEHandle9IsPlayingEv");
    GET_FUNCTION(ds::snd::SEHandle::Play, "_ZN2ds3snd8SEHandle4PlayEiiii");
    GET_FUNCTION(ds::snd::SEHandle::PlayFadeIn, "_ZN2ds3snd8SEHandle10PlayFadeInEiiiii");
    GET_FUNCTION(ds::snd::SEHandle::setPan, "_ZN2ds3snd8SEHandle6setPanEti");
    GET_FUNCTION(ds::snd::SEHandle::setPanRange, "_ZN2ds3snd8SEHandle11setPanRangeEti");
    GET_FUNCTION(ds::snd::SEHandle::setPitch, "_ZN2ds3snd8SEHandle8setPitchEti");
    GET_FUNCTION(ds::snd::SEHandle::setTempo, "_ZN2ds3snd8SEHandle8setTempoEi");
    GET_FUNCTION(ds::snd::SEHandle::setVolume, "_ZN2ds3snd8SEHandle9setVolumeEi");
    GET_FUNCTION(ds::snd::SEHandle::MoveVolume, "_ZN2ds3snd8SEHandle9setVolumeEii");
    GET_FUNCTION(ds::snd::SEHandle::stop, "_ZN2ds3snd8SEHandle4stopEi");

    GET_FUNCTION(ds::sys3d::CAnimation::ctor, "_ZN2ds5sys3d10CAnimationC1Ev");
    GET_FUNCTION(ds::sys3d::CAnimation::getFrameRate, "_ZN2ds5sys3d10CAnimation12getFrameRateEv");
    GET_FUNCTION(ds::sys3d::CAnimation::setup, "_ZN2ds5sys3d10CAnimation5setupEPvS2_jS2_");
    GET_FUNCTION(ds::sys3d::CAnimation::setFrameRate, "_ZN2ds5sys3d10CAnimation12setFrameRateEi");
    GET_FUNCTION(ds::sys3d::CMotSet::getFrameRate, "_ZN2ds5sys3d7CMotSet12getFrameRateEv");
    GET_FUNCTION(ds::sys3d::CMotSet::setFrameRate, "_ZN2ds5sys3d7CMotSet12setFrameRateEi");

    GET_FUNCTION(ds::CPad::repeat, "_ZN2ds4CPad6repeatEv");
    GET_VARIABLE(ds::g_Pad, "_ZN2ds5g_PadE");

    GET_FUNCTION(egs::GilbartInitialize, "_ZN3egs17GilbartInitializeEbb");
    GET_FUNCTION(egs::GilbartTerminate, "_ZN3egs16GilbartTerminateEv");
    GET_FUNCTION(egs::GilbartIsPlaying, "_ZN3egs16GilbartIsPlayingEi");
    GET_FUNCTION(egs::GilbartIsEndOfPlayVoice, "_ZN3egs23GilbartIsEndOfPlayVoiceEi");
    GET_FUNCTION(egs::GilbartSetVolume, "_ZN3egs16GilbartSetVolumeEif");
    GET_FUNCTION(egs::GilbartPauseVoice, "_ZN3egs17GilbartPauseVoiceEib");
    GET_FUNCTION(egs::GilbartPlayVoice, "_ZN3egs16GilbartPlayVoiceEPKci");
    GET_FUNCTION(egs::GilbartStopVoice, "_ZN3egs16GilbartStopVoiceEi");
    GET_FUNCTION(egs::GilbartStatus, "_ZN3egs13GilbartStatusEi");
    GET_FUNCTION(egs::GilbartProcess, "_ZN3egs14GilbartProcessEPv");

    GET_VARIABLE(texCount, "texCount");
    GET_VARIABLE(memCount, "memCount");

    GET_FUNCTION(Font::initFont, "_ZN4Font8initFontEv");

    GET_FUNCTION(dgs::DGSMsdGetString, "_ZN3dgs15DGSMsdGetStringEjhPNS_6DGSMSDE");

    GET_FUNCTION(title::TitleContents::update, "_ZN5title13TitleContents6updateEv");

    GET_FUNCTION(title::TitleSubState::update, "_ZN5title13TitleSubState6updateEv");

    GET_FUNCTION(FlagManager::singleton, "_ZN11FlagManager9singletonEv");
    GET_FUNCTION(FlagManager::Ctor, "_ZN11FlagManagerC1Ev");
    GET_FUNCTION(FlagManager::Dtor, "_ZN11FlagManagerD1Ev");
    GET_FUNCTION(FlagManager::get, "_ZN11FlagManager3getEjj");
    GET_FUNCTION(FlagManager::set, "_ZN11FlagManager3setEjj");
    GET_FUNCTION(FlagManager::reset, "_ZN11FlagManager5resetEjj");
    GET_FUNCTION(FlagManager::reverse, "_ZN11FlagManager7reverseEjj");
    GET_VARIABLE(FlagManager::_instance, "_ZN11FlagManager9_instanceE");

    GET_FUNCTION(AchievementContext::getArchiveReportArray, "_ZN18AchievementContext21getArchiveReportArrayEv");
    GET_VARIABLE(AchievementContext::instance_, "_ZN18AchievementContext9instance_E");

    GET_FUNCTION(AchievementReporter::Ctor, "_ZN19AchievementReporterC1Ev");
    GET_FUNCTION(AchievementReporter::Dtor, "_ZN19AchievementReporterD1Ev");
    GET_FUNCTION(AchievementReporter::init, "_ZN19AchievementReporter4initEv");
    GET_FUNCTION(AchievementReporter::update, "_ZN19AchievementReporter6updateEv");
    GET_FUNCTION(AchievementReporter::addParam, "_ZN19AchievementReporter8addParamER16AchievementParam");
    GET_FUNCTION(AchievementReporter::finish, "_ZN19AchievementReporter6finishEPKc");
    GET_FUNCTION(AchievementReporter::isAllReported, "_ZNK19AchievementReporter13isAllReportedEv");
    GET_FUNCTION(AchievementReporter::isReportedSecondReport, "_ZN19AchievementReporter22isReportedSecondReportEv");
    GET_FUNCTION(AchievementReporter::eraseReported, "_ZN19AchievementReporter13eraseReportedEv");
    GET_FUNCTION(AchievementReporter::syncSaveData, "_ZN19AchievementReporter12syncSaveDataEP19AchievementResource");

    GET_VARIABLE(AchievementCheckFuncs::DecantItemTbl, "_ZN21AchievementCheckFuncs13DecantItemTblE");

    GET_FUNCTION(AchievementCheckFuncs::Ctor, "_ZN21AchievementCheckFuncsC1Ev");
    GET_FUNCTION(AchievementCheckFuncs::Dtor, "_ZN21AchievementCheckFuncsD1Ev");
    GET_FUNCTION(AchievementCheckFuncs::init, "_ZN21AchievementCheckFuncs4initEP19AchievementResource");
    GET_FUNCTION(AchievementCheckFuncs::clearCacheParam, "_ZN21AchievementCheckFuncs15clearCacheParamEv");
    GET_FUNCTION(AchievementCheckFuncs::callFunc, "_ZN21AchievementCheckFuncs8callFuncEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::getDecantItemName, "_ZN21AchievementCheckFuncs17getDecantItemNameEiPc");

    GET_FUNCTION(AchievementCheckFuncs::flag, "_ZN21AchievementCheckFuncs4flagEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::flagRange, "_ZN21AchievementCheckFuncs9flagRangeEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::hardModeBeat, "_ZN21AchievementCheckFuncs12hardModeBeatEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::beatCount, "_ZN21AchievementCheckFuncs9beatCountEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::getItem, "_ZN21AchievementCheckFuncs7getItemEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::getDecant, "_ZN21AchievementCheckFuncs9getDecantEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::getDecantCnt, "_ZN21AchievementCheckFuncs12getDecantCntEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::learnSummon, "_ZN21AchievementCheckFuncs11learnSummonEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::learnSummonCnt, "_ZN21AchievementCheckFuncs14learnSummonCntEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::completedMap, "_ZN21AchievementCheckFuncs12completedMapEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::overPlayerLevel, "_ZN21AchievementCheckFuncs15overPlayerLevelEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::gill, "_ZN21AchievementCheckFuncs4gillEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::monsterKillCount, "_ZN21AchievementCheckFuncs16monsterKillCountEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::monsterDictionary, "_ZN21AchievementCheckFuncs17monsterDictionaryEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::monsterKillParty, "_ZN21AchievementCheckFuncs16monsterKillPartyEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::noCheck, "_ZN21AchievementCheckFuncs7noCheckEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::field, "_ZN21AchievementCheckFuncs5fieldEP16AchievementParam");
    GET_FUNCTION(AchievementCheckFuncs::achievementComp, "_ZN21AchievementCheckFuncs15achievementCompEP16AchievementParam");

    GET_FUNCTION(AchievementChecker::Ctor, "_ZN18AchievementCheckerC1Ev");
    GET_FUNCTION(AchievementChecker::Dtor, "_ZN18AchievementCheckerD1Ev");
    GET_FUNCTION(AchievementChecker::init, "_ZN18AchievementChecker4initEP19AchievementReporterP19AchievementResourceP21AchievementCheckFuncs");
    GET_FUNCTION(AchievementChecker::update, "_ZN18AchievementChecker6updateE25enAchievementCheckTimings");

    GET_VARIABLE(mr::getCompleteRate, "_ZN2mr15setCompleteRateEv");

    GET_FUNCTION(mon::MonsterManiaManager::monsterMania, "_ZN3mon19MonsterManiaManager12monsterManiaEi");
    GET_FUNCTION(mon::MonsterManiaManager::onEncount, "_ZN3mon19MonsterManiaManager9onEncountEs");
    GET_FUNCTION(mon::MonsterManiaManager::onLibrary, "_ZN3mon19MonsterManiaManager9onLibraryEs");
    GET_FUNCTION(mon::MonsterManiaManager::kill, "_ZN3mon19MonsterManiaManager4killEs");
    GET_FUNCTION(mon::MonsterManiaManager::clearMonsterMania, "_ZN3mon19MonsterManiaManager17clearMonsterManiaEv");
    GET_FUNCTION(mon::MonsterManiaManager::setMonsterManiaForMonsterID, "_ZN3mon19MonsterManiaManager27setMonsterManiaForMonsterIDERNS_12MonsterManiaE");

    GET_FUNCTION(mon::MonsterManager::load, "_ZN3mon14MonsterManager4loadEv");
    GET_FUNCTION(mon::MonsterManager::free, "_ZN3mon14MonsterManager4freeEv");
    GET_FUNCTION(mon::MonsterManager::monsterParameter, "_ZN3mon14MonsterManager16monsterParameterEi");
    GET_FUNCTION(mon::MonsterManager::dropItem, "_ZN3mon14MonsterManager8dropItemEi");
    GET_FUNCTION(mon::MonsterManager::ai, "_ZN3mon14MonsterManager2aiEi");
    GET_FUNCTION(mon::MonsterManager::counter, "_ZN3mon14MonsterManager7counterEi");
    GET_FUNCTION(mon::MonsterManager::turnAction, "_ZN3mon14MonsterManager10turnActionEi");
    GET_FUNCTION(mon::MonsterManager::actionCondition, "_ZN3mon14MonsterManager15actionConditionEi");
    GET_FUNCTION(mon::MonsterManager::offset, "_ZN3mon14MonsterManager6offsetEi");
    GET_FUNCTION(mon::MonsterManager::normalAttack, "_ZN3mon14MonsterManager12normalAttackEi");
    GET_FUNCTION(mon::MonsterManager::octmanmosLegInfo, "_ZN3mon14MonsterManager16octmanmosLegInfoEhh");
    GET_FUNCTION(mon::MonsterManager::effectsInfo, "_ZN3mon14MonsterManager11effectsInfoEN6common10ABILITY_IDEs");
    GET_FUNCTION(mon::MonsterManager::acceptMonsterManiaData, "_ZN3mon14MonsterManager22acceptMonsterManiaDataERNS_19MonsterManiaManagerE");
    GET_FUNCTION(mon::MonsterManager::monsterManiaManager, "_ZN3mon14MonsterManager19monsterManiaManagerEv");
    GET_FUNCTION(mon::MonsterManager::sendMonsterManiaData, "_ZN3mon14MonsterManager20sendMonsterManiaDataERNS_19MonsterManiaManagerE");
    GET_FUNCTION(mon::MonsterManager::setMonsterIdForMonsterManaia, "_ZN3mon14MonsterManager28setMonsterIdForMonsterManaiaEv");
    GET_VARIABLE(mon::MonsterManager::instance_, "_ZN3mon14MonsterManager9instance_E");

    GET_FUNCTION(mr::MonsterRefDataManager::Ctor, "_ZN2mr21MonsterRefDataManagerC1Ev");
    GET_FUNCTION(mr::MonsterRefDataManager::Dtor, "_ZN2mr21MonsterRefDataManagerD1Ev");
    GET_FUNCTION(mr::MonsterRefDataManager::load, "_ZN2mr21MonsterRefDataManager4loadEv");
    GET_FUNCTION(mr::MonsterRefDataManager::free, "_ZN2mr21MonsterRefDataManager4freeEv");
    GET_FUNCTION(mr::MonsterRefDataManager::monsterReference, "_ZN2mr21MonsterRefDataManager16monsterReferenceEi");
    GET_FUNCTION(mr::MonsterRefDataManager::monsterReferenceFromIndex, "_ZN2mr21MonsterRefDataManager25monsterReferenceFromIndexEi");
    GET_VARIABLE(mr::MRDMng, "_ZN2mr6MRDMngE");

    GET_FUNCTION(itm::ItemManager::consumptionParameter, "_ZN3itm11ItemManager20consumptionParameterEs");
    GET_FUNCTION(itm::ItemManager::weaponParameter, "_ZN3itm11ItemManager15weaponParameterEs");
    GET_FUNCTION(itm::ItemManager::protectionParameter, "_ZN3itm11ItemManager19protectionParameterEs");
    GET_FUNCTION(itm::ItemManager::importantParameter, "_ZN3itm11ItemManager18importantParameterEs");
    GET_FUNCTION(itm::ItemManager::equipParameter, "_ZN3itm11ItemManager14equipParameterEs");
    GET_FUNCTION(itm::ItemManager::itemParameter, "_ZN3itm11ItemManager13itemParameterEs");
    GET_FUNCTION(itm::ItemManager::itemCategory, "_ZN3itm11ItemManager12itemCategoryEs");
    GET_FUNCTION(itm::ItemManager::load, "_ZN3itm11ItemManager4loadEv");
    GET_FUNCTION(itm::ItemManager::free, "_ZN3itm11ItemManager4freeEv");
    GET_VARIABLE(itm::ItemManager::instance_, "_ZN3itm11ItemManager9instance_E");
    GET_FUNCTION(itm::PossessionItemManager::normalItem, "_ZN3itm21PossessionItemManager10normalItemEi");
    GET_FUNCTION(itm::PossessionItemManager::itemAllNum, "_ZN3itm21PossessionItemManager10itemAllNumEv");
    GET_FUNCTION(itm::PossessionItemManager::resetItemId, "_ZN3itm21PossessionItemManager11resetItemIdEv");
    GET_FUNCTION(itm::PossessionItemManager::initialize, "_ZN3itm21PossessionItemManager10initializeEv");
    GET_FUNCTION(itm::PossessionItemManager::importantItem, "_ZN3itm21PossessionItemManager13importantItemEi");
    GET_FUNCTION(itm::PossessionItemManager::storeItem, "_ZN3itm21PossessionItemManager9storeItemEsi");
    GET_FUNCTION(itm::PossessionItemManager::searchNormalItem, "_ZN3itm21PossessionItemManager16searchNormalItemEs");
    GET_FUNCTION(itm::PossessionItemManager::normalItemNum, "_ZN3itm21PossessionItemManager13normalItemNumEv");
    GET_FUNCTION(itm::PossessionItemManager::importantItemNum, "_ZN3itm21PossessionItemManager16importantItemNumEv");
    GET_FUNCTION(itm::PossessionItemManager::eraseAllNormalItem, "_ZN3itm21PossessionItemManager18eraseAllNormalItemEv");
    GET_FUNCTION(itm::PossessionItemManager::reserveCancel, "_ZN3itm21PossessionItemManager13reserveCancelEv");
    GET_FUNCTION(itm::PossessionItemManager::resetImportantItemId, "_ZN3itm21PossessionItemManager20resetImportantItemIdEv");
    GET_FUNCTION(itm::PossessionItemManager::swapItem, "_ZN3itm21PossessionItemManager8swapItemEss");
    GET_FUNCTION(itm::PossessionItemManager::eraseItem, "_ZN3itm21PossessionItemManager9eraseItemEsi");
    GET_FUNCTION(itm::PossessionItemManager::eraseAllImportantItem, "_ZN3itm21PossessionItemManager21eraseAllImportantItemEv");
    GET_FUNCTION(itm::PossessionItemManager::storeImportantItem, "_ZN3itm21PossessionItemManager18storeImportantItemEsi");
    GET_FUNCTION(itm::PossessionItemManager::searchImportantItem, "_ZN3itm21PossessionItemManager19searchImportantItemEs");

    GET_FUNCTION(common::EfficacyDataConvection::Ctor, "_ZN6common22EfficacyDataConvectionC2Ev");
    GET_FUNCTION(common::EfficacyDataConvection::Dtor, "_ZN6common22EfficacyDataConvectionD2Ev");
    GET_FUNCTION(common::EfficacyDataConvection::initialize, "_ZN6common22EfficacyDataConvection10initializeEv");
    GET_FUNCTION(common::EfficacyDataConvection::loadBELD, "_ZN6common22EfficacyDataConvection8loadBELDEv");
    GET_FUNCTION(common::EfficacyDataConvection::unloadBELD, "_ZN6common22EfficacyDataConvection10unloadBELDEv");
    GET_FUNCTION(common::EfficacyDataConvection::terminate, "_ZN6common22EfficacyDataConvection9terminateEv");
    GET_FUNCTION(common::EfficacyDataConvection::getEfficacyData, "_ZN6common22EfficacyDataConvection15getEfficacyDataEi");
    GET_VARIABLE(common::EfficacyDataConvection::instance_, "_ZN6common22EfficacyDataConvection9instance_E");

    GET_FUNCTION(pl::LearningAbilityManager::initialize, "_ZN2pl22LearningAbilityManager10initializeERNS_30LearningAbilityManagerContentsE");
    GET_FUNCTION(pl::LearningAbilityManager::setContents, "_ZN2pl22LearningAbilityManager11setContentsERNS_30LearningAbilityManagerContentsE");
    GET_FUNCTION(pl::LearningAbilityManager::on, "_ZN2pl22LearningAbilityManager2onENS0_22LEARNING_ABILITY_TYPESEi");
    GET_FUNCTION(pl::LearningAbilityManager::off, "_ZN2pl22LearningAbilityManager3offENS0_22LEARNING_ABILITY_TYPESEi");
    GET_FUNCTION(pl::LearningAbilityManager::isOn, "_ZN2pl22LearningAbilityManager4isOnENS0_22LEARNING_ABILITY_TYPESEi");
    GET_FUNCTION(pl::LearningAbilityManager::clear, "_ZN2pl22LearningAbilityManager5clearENS0_22LEARNING_ABILITY_TYPESE");
    GET_FUNCTION(pl::LearningAbilityManager::isMagicTypesAbility, "_ZN2pl22LearningAbilityManager19isMagicTypesAbilityEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::LearningAbilityManager::learningMagicTypesNumber, "_ZN2pl22LearningAbilityManager24learningMagicTypesNumberEv");
    GET_FUNCTION(pl::LearningAbilityManager::learn, "_ZN2pl22LearningAbilityManager5learnEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::LearningAbilityManager::learnForDebug, "_ZN2pl22LearningAbilityManager13learnForDebugEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::LearningAbilityManager::forget, "_ZN2pl22LearningAbilityManager6forgetEN6common10ABILITY_IDE");
    GET_FUNCTION(pl::LearningAbilityManager::isLearning, "_ZN2pl22LearningAbilityManager10isLearningEN6common10ABILITY_IDE");
    GET_VARIABLE(pl::LearningAbilityManager::FLAG_SIZE, "_ZN2pl22LearningAbilityManager9FLAG_SIZEE");
    GET_VARIABLE(pl::LearningAbilityManager::ARRAY_SIZE, "_ZN2pl22LearningAbilityManager10ARRAY_SIZEE");
    GET_VARIABLE(pl::LearningAbilityManager::FLAG_OFFSET, "_ZN2pl22LearningAbilityManager11FLAG_OFFSETE");

    GET_FUNCTION(ds::sys3d::CModelTexture::Ctor, "_ZN2ds5sys3d13CModelTextureC1Ev");
    GET_FUNCTION(ds::sys3d::CModelTexture::Dtor, "_ZN2ds5sys3d13CModelTextureD1Ev");
    GET_FUNCTION(ds::sys3d::CModelTexture::clearValue, "_ZN2ds5sys3d13CModelTexture10clearValueEv");
    GET_FUNCTION(ds::sys3d::CModelTexture::setup, "_ZN2ds5sys3d13CModelTexture5setupEPvb");
    GET_FUNCTION(ds::sys3d::CModelTexture::cleanup, "_ZN2ds5sys3d13CModelTexture7cleanupEv");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseResource, "_ZN2ds5sys3d13CModelTexture15releaseResourceEv");
    GET_FUNCTION(ds::sys3d::CModelTexture::reqReleaseResource, "_ZN2ds5sys3d13CModelTexture18reqReleaseResourceEv");
    GET_FUNCTION(ds::sys3d::CModelTexture::tdlhCompletion, "_ZN2ds5sys3d13CModelTexture14tdlhCompletionEi");
    GET_FUNCTION(ds::sys3d::CModelTexture::isModelTexture, "_ZN2ds5sys3d13CModelTexture14isModelTextureEPv");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdl, "_ZN2ds5sys3d13CModelTexture7bindMdlEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdl, "_ZN2ds5sys3d13CModelTexture10releaseMdlEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdlSet, "_ZN2ds5sys3d13CModelTexture10bindMdlSetEP15NNSG3dResMdlSet");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdlSet, "_ZN2ds5sys3d13CModelTexture13releaseMdlSetEP15NNSG3dResMdlSet");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdlToTex, "_ZN2ds5sys3d13CModelTexture12bindMdlToTexEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdlToTex, "_ZN2ds5sys3d13CModelTexture15releaseMdlToTexEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdlToTexByName, "_ZN2ds5sys3d13CModelTexture18bindMdlToTexByNameEP12NNSG3dResMdlPKc");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdlToTexByName, "_ZN2ds5sys3d13CModelTexture21releaseMdlToTexByNameEP12NNSG3dResMdlPKc");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdlToPltt, "_ZN2ds5sys3d13CModelTexture13bindMdlToPlttEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdlToPltt, "_ZN2ds5sys3d13CModelTexture16releaseMdlToPlttEP12NNSG3dResMdl");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindMdlToPlttByName, "_ZN2ds5sys3d13CModelTexture19bindMdlToPlttByNameEP12NNSG3dResMdlPKc");
    GET_FUNCTION(ds::sys3d::CModelTexture::releaseMdlToPlttByName, "_ZN2ds5sys3d13CModelTexture22releaseMdlToPlttByNameEP12NNSG3dResMdlPKc");
    GET_FUNCTION(ds::sys3d::CModelTexture::bindTexPlt, "_ZN2ds5sys3d13CModelTexture10bindTexPltEv");

    GET_FUNCTION(CTextureDataMng::init, "_ZN15CTextureDataMng4initEv");
    GET_FUNCTION(CTextureDataMng::end, "_ZN15CTextureDataMng3endEv");
    GET_FUNCTION(CTextureDataMng::initValue, "_ZN15CTextureDataMng9initValueEi");
    GET_FUNCTION(CTextureDataMng::setTextureData, "_ZN15CTextureDataMng14setTextureDataEPKcb");
    GET_FUNCTION(CTextureDataMng::setData, "_ZN15CTextureDataMng7setDataEPKcb");
    GET_FUNCTION(CTextureDataMng::setData2, "_ZN15CTextureDataMng8setData2EPKcS1_b");
    GET_FUNCTION(CTextureDataMng::searchDataIndex, "_ZN15CTextureDataMng15searchDataIndexEPKc");
    GET_FUNCTION(CTextureDataMng::searchNullIndex, "_ZN15CTextureDataMng15searchNullIndexEv");
    GET_FUNCTION(CTextureDataMng::useTexture, "_ZN15CTextureDataMng10useTextureEi");
    GET_FUNCTION(CTextureDataMng::delData, "_ZN15CTextureDataMng7delDataEi");
    GET_FUNCTION(CTextureDataMng::getTex, "_ZN15CTextureDataMng6getTexEi");
    GET_FUNCTION(CTextureDataMng::setupLnkTexVram, "_ZN15CTextureDataMng15setupLnkTexVramEib");
    GET_FUNCTION(CTextureDataMng::releaseLnkTexVram, "_ZN15CTextureDataMng17releaseLnkTexVramEi");

    GET_FUNCTION(itm::EquipParameter::canEquip, "_ZN3itm14EquipParameter8canEquipEN2pl12PLAYER_TYPESE");

    GET_FUNCTION(btl::BaseBattleCharacter::isInvincible, "_ZN3btl19BaseBattleCharacter12isInvincibleEv");

    GET_FUNCTION(btl::BattleDebugParameter::flag, "_ZN3btl20BattleDebugParameter4flagENS_10DEBUG_FLAGE");

    GET_FUNCTION(debug::CharaViewerPart::doInitialize, "_ZN5debug15CharaViewerPart12doInitializeEv");

    GET_FUNCTION(ds::CVram::setupTexVramMng, "_ZN2ds5CVram15setupTexVramMngEjjjc");
    GET_FUNCTION(ds::CVram::setupPlttVramMng, "_ZN2ds5CVram16setupPlttVramMngEjjc");
    GET_FUNCTION(ds::CVram::releaseTexVramMng, "_ZN2ds5CVram17releaseTexVramMngEv");
    GET_FUNCTION(ds::CVram::releasePlttVramMng, "_ZN2ds5CVram18releasePlttVramMngEv");
    GET_VARIABLE(ds::CVram::m_instance, "_ZN2ds5CVram10m_instanceE");

    GET_VARIABLE(evt::EventCamera::startCameraMotion, "_ZN3evt11EventCamera17startCameraMotionEjjjb");

    GET_VARIABLE(ds::sys3d::CCamera::setFOV, "_ZN2ds5sys3d7CCamera6setFOVEii");

    GET_FUNCTION(ds::snd::dssndUnloadSE1, "_ZN2ds3snd13dssndUnloadSEEv");
    GET_FUNCTION(ds::snd::dssndUnloadSE2, "_ZN2ds3snd13dssndUnloadSEEi");
    GET_FUNCTION(ds::snd::dssndUnloadSEAll, "_ZN2ds3snd16dssndUnloadSEAllEv");

    GET_FUNCTION(ds::snd::dssndUnloadBGM1, "_ZN2ds3snd14dssndUnloadBGMEv");
    GET_FUNCTION(ds::snd::dssndUnloadBGM2, "_ZN2ds3snd14dssndUnloadBGMEi");
    GET_FUNCTION(ds::snd::dssndUnloadBGMAll, "_ZN2ds3snd17dssndUnloadBGMAllEv");
}
}