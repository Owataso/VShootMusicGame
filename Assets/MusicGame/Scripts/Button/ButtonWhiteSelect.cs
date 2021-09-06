using UnityEngine;
using UnityEngine.UI;

public class ButtonWhiteSelect : BaseButton
{
    public Image image;
    SpriteRenderer renderer;

    protected override void Awake()
    {
        base.Awake();

        renderer = GetComponent<SpriteRenderer>();
    }

    public override bool OnRayHitEnter()
    {
        if (base.OnRayHitEnter())
        {
            // 画像の色を若干白にする
            if (renderer) renderer.color = new Color(0.75f, 0.75f, 0.75f);
            else image.color = new Color(0.75f, 0.75f, 0.75f);

            return true;
        }
        return false;
    }

    public override void OnRayHitExit()
    {
        base.OnRayHitExit();

        // 画像の色を元に戻す
        if (renderer) renderer.color = new Color(0.5f, 0.5f, 0.5f);
        else image.color = new Color(0.5f, 0.5f, 0.5f);
    }
}
