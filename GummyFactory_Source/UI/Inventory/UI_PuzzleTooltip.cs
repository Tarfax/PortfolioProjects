using TMPro;
using UnityEngine;
using UnityEngine.UI;

#if UNITY_EDITOR
[ExecuteInEditMode]
#endif
public class UI_PuzzleTooltip : MonoBehaviour {

    [SerializeField] private RectTransform backgroundRectTransform;
    [SerializeField] private TextMeshProUGUI textContent;
    [SerializeField] private LayoutElement layoutElement;
    [SerializeField] private int characterWrapLimit = 40;
    //[SerializeField] private float heightOffset = 150f;
    [SerializeField] private RectTransform rectTransform;

    void Update() {
        if (Application.isEditor == true) {
            int contentLength = textContent.text.Length;

            layoutElement.enabled = contentLength > characterWrapLimit ? true : false;
        }
    }

    public void SetText(InteractableDefinition interactableDefinition) {
        textContent.SetText(interactableDefinition.Description);
        textContent.ForceMeshUpdate();

        int contentLength = textContent.text.Length;

        layoutElement.enabled = contentLength > characterWrapLimit ? true : false;
    }

    public void SetPositionWorld(Vector2 position, float heightOffset) {
        float pivotX = position.x / Screen.width;
        pivotX = 0.5f;
        pivotX = Mathf.Clamp(pivotX, 0.05f, 0.95f);
        float pivotY = position.y / Screen.height;

        rectTransform.pivot = new Vector2(pivotX, pivotY);

        transform.position = new Vector2(position.x, position.y + heightOffset);
    }

    public void SetActive(bool activate) {
        gameObject.SetActive(activate);
    }


}
