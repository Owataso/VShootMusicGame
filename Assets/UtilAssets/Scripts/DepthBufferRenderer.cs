using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DepthBufferRenderer : MonoBehaviour
{
    RenderTexture colorTexture;
    RenderTexture depthTexture;
    public Material postRenderMaterial;

	// Use this for initialization
	void Start ()
    {
        var camera = GetComponent<Camera>();
        camera.depthTextureMode = DepthTextureMode.Depth;
        // カラーバッファ用
        colorTexture = new RenderTexture(Screen.width, Screen.height, 0, RenderTextureFormat.ARGB32);
        colorTexture.Create();
        // デプスバッファ用用
        depthTexture = new RenderTexture(Screen.width, Screen.height, 24, RenderTextureFormat.Depth);
        camera.SetTargetBuffers(colorTexture.colorBuffer, depthTexture.depthBuffer);
	}

    void OnPostRender()
    {
        // レンダーターゲットなし:画面に出力される
        Graphics.SetRenderTarget(null);
        // デプスバッファを描画する(postRenderMaterialはテクスチャ画像をそのまま描画するマテリアル)
        Graphics.Blit(depthTexture, postRenderMaterial);
    }
}
