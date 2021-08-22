using UnityEngine;
using TMPro;
using UnityEngine.UI;

#if UNITY_EDITOR
[ExecuteInEditMode]
#endif
public class UI_Tooltip : MonoBehaviour {

    [SerializeField] private RectTransform backgroundRectTransform;
    [SerializeField] private TextMeshProUGUI textHeader;
    [SerializeField] private TextMeshProUGUI textContent;
    [SerializeField] private LayoutElement layoutElement;
    [SerializeField] private int headerCharacterWrapLimit = 14;
    [SerializeField] private int characterWrapLimit = 35;
    [SerializeField] private RectTransform rectTransform;

    [SerializeField] private bool followMouse;

    private void Update() {
        if (Application.isEditor == true) {
            int headerLength = textHeader.text.Length;
            int contentLength = textContent.text.Length;

            layoutElement.enabled = (headerLength > headerCharacterWrapLimit || contentLength > characterWrapLimit) ? true : false;
        }

        if (followMouse == true) {
            Vector2 position = Input.mousePosition;

            float pivotX = position.x / Screen.width;
            float pivotY = position.y / Screen.height;

            rectTransform.pivot = new Vector2(pivotX, pivotY);

            transform.position = position;
        }
    }

    public void SetText(string tooltipText) {
        textContent.SetText(tooltipText);
        textContent.ForceMeshUpdate();

        Vector2 textSize = textContent.GetRenderedValues(false);
        Vector2 paddingSize = new Vector2(8, 8);

        backgroundRectTransform.sizeDelta = textSize + paddingSize;
    }

    public void SetText(ItemDefinition itemDefinition) {
        textHeader.SetText(itemDefinition.ItemTitle);
        textContent.SetText(itemDefinition.ItemDescription);
        textContent.ForceMeshUpdate();

        int headerLength = textHeader.text.Length;
        int contentLength = textContent.text.Length;

        layoutElement.enabled = (headerLength > headerCharacterWrapLimit || contentLength > characterWrapLimit) ? true : false;
    }


    public void SetPositionUI(Vector2 transformPosition, float heightOffset) {
        followMouse = false;

        Vector2 position = transformPosition;

        float pivotX = position.x / Screen.width;
        pivotX = Mathf.Clamp(pivotX, 0.15f, 0.85f);
        float pivotY = position.y / Screen.height;

        rectTransform.pivot = new Vector2(pivotX, pivotY);

        transform.position = new Vector2(position.x, position.y + heightOffset);
    }

    public void SetActive(bool activate) {
        gameObject.SetActive(activate);
    }

}
