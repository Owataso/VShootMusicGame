using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// ゲームを起動しなくても稼働する(つまり値を変えながらのプレビューが可能)
//[ExecuteInEditMode]
public class TransitionDiferred : MonoBehaviour
{
    public Material shaderMaterial;

    TransitionForward transitionForward;

    void Awake()
    {
        //shaderMaterial = new Material(transitionShader);
        transitionForward = transform.parent.GetComponent<TransitionForward>();
    }


    void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        // シェーダーに緑を描画していない写真を渡す
        shaderMaterial.SetTexture("_PutTex", transitionForward.backUp);

        // 描画
        Graphics.Blit(source, destination, shaderMaterial);

        // その上に歪み用オブジェクトがない写真をシェーダーで乗せる
        //Graphics.Blit(transitionForward.backUp, destination);
    }
}
