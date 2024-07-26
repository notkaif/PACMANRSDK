// ---------------------------------------------------------------------
// RSDK Project: PACMANRSDK
// Object Description: PACMANGame Object
// Object Author: notkaif
// ---------------------------------------------------------------------

#include "PACMANGame.hpp"
#include "PacmanPlayer.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(PACMANGame);

void PACMANGame::Update(void)
{
    /* if (sceneInfo->state == ENGINESTATE_FROZEN) {
        for (int8 i = Input::CONT_P1; i <= Input::CONT_P2; i++) {
            ControllerState *controller = &controllerInfo[i];
            Paddle *player1             = RSDK_GET_ENTITY(SLOT_PLAYER1, Paddle);
            Paddle *player2             = RSDK_GET_ENTITY(SLOT_PLAYER2, Paddle);

            if (player1->score >= PONGGAME_WINSCORE || player2->score >= PONGGAME_WINSCORE) {
                if (controller->keyStart.press || (API::GetConfirmButtonFlip() ? controller->keyB.press : controller->keyA.press)) {
                    player1->score = 0;
                    player2->score = 0;
                    ResetField(nullptr);
                    Stage::SetEngineState(ENGINESTATE_REGULAR);
                }
                else if (API::GetConfirmButtonFlip() ? controller->keyA.press : controller->keyB.press)
                    API::ExitGame();
            }
            else if (controller->keyStart.press)
                Stage::SetEngineState(ENGINESTATE_REGULAR);
        }
    }
    */
}

void PACMANGame::LateUpdate(void) {}
void PACMANGame::StaticUpdate(void) {}

void PACMANGame::Draw(void)
{
    if (sceneInfo->state == ENGINESTATE_FROZEN) {
        RSDK::Vector2 position;
        position.x = TO_FIXED(screenInfo->center.x);
        position.y = TO_FIXED(screenInfo->center.y);
        this->animator.DrawSprite(&position, true);
    }
}

void PACMANGame::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_ALWAYS;
        this->visible       = true;
        this->drawGroup     = 2;
        this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
    }
}

void PACMANGame::StageLoad(void)
{
    foreach_all(PACMANGame, spawn)
    {
        PACMANGame *pongGame = RSDK_GET_ENTITY(SLOT_PACMANGAME, PACMANGame);
        spawn->Copy(pongGame, true);
    }

    sVars->aniFrames.Load("Entities/Pacman.bin", SCOPE_STAGE);
}

#if GAME_INCLUDE_EDITOR
void PACMANGame::EditorDraw(void) {}

void PACMANGame::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void PACMANGame::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(PACMANGame); }
#endif

void PACMANGame::Serialize(void) {}

} // namespace GameLogic
