using _Game.Scripts.Actors;
using _Game.Scripts.Systems.Interactables;
using UnityEngine;

namespace _Game.Scripts.Player
{
    public class PlayerController : MonoBehaviour
    {
        [SerializeField] private Actor playerCharacter1;
        [SerializeField] private Actor playerCharacter2;
        [Space(10)]
        [SerializeField] private Camera mainCamera;

        private const int uiLayer = 5;
        private const int playerCharacter1Layer = 6;
        private const int playerCharacter2Layer = 7;
        private const int walkableLayer1 = 8;
        private const int walkableLayer2 = 9;
        private const int interactableLayer1 = 10;
        private const int interactableLayer2 = 11;
        private const int obstacleLayer = 12;

        private Vector2 mouseScreenPosition;

        private LayerMask combinedLayerMask;

        public static Actor ActorLeft;
        public static Actor ActorRight;

        private void OnValidate()
        {
            if(mainCamera == null)
                mainCamera = Camera.main;
            if (playerCharacter1 == null)
                playerCharacter1 = GameObject.FindWithTag("Player1").GetComponent<Actor>();
            if (playerCharacter2 == null)
                playerCharacter2 = GameObject.FindWithTag("Player2").GetComponent<Actor>();
        }

        private void Awake()
        {
            combinedLayerMask = 1 << uiLayer | 1 << walkableLayer1 | 1 << walkableLayer2 | 1 << interactableLayer1 |
                                1 << interactableLayer2 | 1 << obstacleLayer;

            ActorLeft = playerCharacter1;
            ActorRight = playerCharacter2;
        }

        private void Update()
        {
            mouseScreenPosition = Input.mousePosition;
            if(Input.GetButtonDown("Fire1"))
                OnLeftMousePressed();
            if(Input.GetButtonDown("Fire2"))
                OnRightMousePressed();
        }

        private void OnLeftMousePressed()
        {
            if(UI_InputController.IsMouseOverUI())
                return;
            Ray ray = mainCamera.ScreenPointToRay(mouseScreenPosition);

            if (Physics.Raycast(ray, out RaycastHit hitInfo, float.MaxValue, combinedLayerMask))
            {
                switch (hitInfo.collider.gameObject.layer)
                {
                    case playerCharacter1Layer: 
                        break;
                    case playerCharacter2Layer: 
                        break;
                    case walkableLayer1: 
                        playerCharacter1.SetTargetPosition(hitInfo.point);
                        break;
                    case walkableLayer2: 
                        playerCharacter2.SetTargetPosition(hitInfo.point);
                        break;
                    case interactableLayer1:
                        if (hitInfo.collider.gameObject.TryGetComponent(out Interactable interactable1))
                            playerCharacter1.MoveToAndInteract(interactable1);
                        break;
                    case interactableLayer2: 
                        if (hitInfo.collider.gameObject.TryGetComponent(out Interactable interactable2))
                            playerCharacter2.MoveToAndInteract(interactable2);
                        break;
                    case obstacleLayer: 
                        break;
                }
            }
        }

        private void OnRightMousePressed()
        {
        }
    }
}