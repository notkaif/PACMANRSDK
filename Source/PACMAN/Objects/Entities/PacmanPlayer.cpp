// ---------------------------------------------------------------------
// RSDK Project: PACMANRSDK
// Object Description: PacmanPlayer Object
// Object Author: notkaif
// ---------------------------------------------------------------------

#include "PacmanPlayer.hpp"
#include "PACMANGame.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(PacmanPlayer);

void PacmanPlayer::Update(void)
{
    this->stateInput.Run(this);

    if (this->up) {
        this->position.y -= TO_FIXED(PADDLE_MOVESPEED);
        if (FROM_FIXED(this->position.y) + sVars->hitbox.top < 0)
            this->position.y = TO_FIXED(sVars->hitbox.bottom);
    }
    else if (this->down) {
        this->position.y += TO_FIXED(PADDLE_MOVESPEED);
        if (FROM_FIXED(this->position.y) + sVars->hitbox.bottom > screenInfo->size.y)
            this->position.y = TO_FIXED(screenInfo->size.y + sVars->hitbox.top);
    }
}

void PacmanPlayer::LateUpdate(void) {}
void PacmanPlayer::StaticUpdate(void) {}

void PacmanPlayer::Draw(void)
{
    Graphics::DrawRect(FROM_FIXED(this->position.x) - PADDLE_WIDTH / 2, FROM_FIXED(this->position.y) - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT,
                       this->paddleColor, 0xFF, INK_NONE, true);
}

void PacmanPlayer::Create(void *data)
{
    if (!sceneInfo->inEditor)
    {
        this->active       = ACTIVE_NORMAL;
        this->visible      = true;
        this->drawGroup    = 2;
        this->playerID     = this->Slot();
        this->controllerID = this->playerID + 1;
        if (this->playerID == SLOT_PLAYER1)
        {
            this->isAI        = false;
            this->paddleColor = 0xFF0000;
            this->position.x  = TO_FIXED(72);
        }
        this->position.y = TO_FIXED(screenInfo->center.y);
        this->stateInput.Set(PacmanPlayer::Input_Player);
        this->originPos = this->position;
        if (this->isAI)
            this->aiTimer = 0;
    }
}

void PacmanPlayer::StageLoad(void)
{
    // Copy Player objects to the beginning of the entity list
    int32 slotID = SLOT_PLAYER;
    foreach_all(PacmanPlayer, player)
    {
        if (slotID < SLOT_PLAYER + 1) {
            player->Copy(RSDK_GET_ENTITY(slotID, PacmanPlayer), true);
            slotID++;
        }
        else
            player->Destroy(); // There should only be 1 player
    }

    sVars->hitbox.left   = -14;
    sVars->hitbox.top    = -14;
    sVars->hitbox.right  = 14;
    sVars->hitbox.bottom = 14;
}

void PacmanPlayer::Input_Player(void)
{
    if (this->controllerID <= Input::CONT_P2) {
        ControllerState *controller = &controllerInfo[this->controllerID];
        AnalogState *stick          = &analogStickInfoL[this->controllerID];

        if (!PACMANGame::sVars->roundStarted)
            PACMANGame::sVars->roundStarted |= controller->keyUp.press | controller->keyDown.press | stick->keyUp.press | stick->keyDown.press;
        else {
            this->up   = controller->keyUp.down;
            this->down = controller->keyDown.down;

            this->up |= stick->keyUp.down;
            this->down |= stick->keyDown.down;

            this->up |= stick->vDelta > 0.3;
            this->down |= stick->vDelta < -0.3;

            if (this->up && this->down) {
                this->up   = false;
                this->down = false;
            }
        }

        if (controller->keyStart.press) {
            RSDK_GET_ENTITY(SLOT_PACMANGAME, PACMANGame)->animator.frameID = 0;
            Stage::SetEngineState(ENGINESTATE_FROZEN);
            controller->keyStart.press = false;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PacmanPlayer::EditorDraw(void)
{
}

void PacmanPlayer::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void PacmanPlayer::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(PacmanPlayer); }
#endif

void PacmanPlayer::Serialize(void) {}

} // namespace GameLogic
