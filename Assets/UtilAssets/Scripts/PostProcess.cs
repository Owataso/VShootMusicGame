using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Camera))]
public class PostProcess : MonoBehaviour
{
    public Material showGBufferMaterial;

	// Use this for initialization
	void Start ()
    {
        GetComponent<Camera>().depthTextureMode |= DepthTextureMode.Depth;
	}

    [ImageEffectOpaque]
    void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        Graphics.Blit(source, destination, showGBufferMaterial);
    }
}
