#pragma once

#include "../../GxNetwork/Include/Common/Network.h"

namespace gx {
namespace network {

/**
 * @brief FObject class forward decl.
 */
class FObject;

/**
 * @brief FObject shared pointer class decl.
 */
typedef std::shared_ptr<FObject> FObjectPtr;

/**
 * @brief FScene class forward decl.
 */
class FScene;

/**
 * @brief FScene shared pointer class decl.
 */
typedef std::shared_ptr<FScene> FScenePtr;

/**
 * @brief FActor class forward decl.
 */
class FActor;

/**
 * @brief FActor shared pointer class decl.
 */
typedef std::shared_ptr<FActor> FActorPtr;

/**
 * @brief FLandscapeActor class forward decl.
 */
class FLandscapeActor;

/**
 * @brief FLandscapeActor shared pointer class decl.
 */
typedef std::shared_ptr<FLandscapeActor> FLandscapeActorPtr;

/**
 * @brief FPawnActor class forward decl.
 */
class FPawnActor;

/**
 * @brief FPawnActor shared pointer class decl.
 */
typedef std::shared_ptr<FPawnActor> FPawnActorPtr;

/**
 * @brief FCharacterActor class forward decl.
 */
class FCharacterActor;

/**
 * @brief FCharacterActor shared pointer class decl.
 */
typedef std::shared_ptr<FCharacterActor> FCharacterActorPtr;

/**
 * @brief FCustomActor class forward decl.
 */
class FCustomActor;

/**
 * @brief FCustomActor shared pointer class decl.
 */
typedef std::shared_ptr<FCustomActor> FCustomActorPtr;

/**
 * @brief FTreeActor class forward decl.
 */
class FTreeActor;

/**
 * @brief FTreeActor shared pointer class decl.
 */
typedef std::shared_ptr<FTreeActor> FTreeActorPtr;

/**
 * @brief FWaterActor class forward decl.
 */
class FWaterActor;

/**
 * @brief FWaterActor shared pointer class decl.
 */
typedef std::shared_ptr<FWaterActor> FWaterActorPtr;

/**
 * @brief FMobActor class forward decl.
 */
class FMobActor;

/**
 * @brief FMobActor shared pointer class decl.
 */
typedef std::shared_ptr<FMobActor> FMobActorPtr;


}
}