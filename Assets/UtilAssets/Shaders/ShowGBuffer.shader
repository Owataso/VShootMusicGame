Shader "OUL/ShowGBuffer"
{
	Properties
	{
		[KeywordEnum(DIFFUSE, SPECULAR, WORLD_SPACE_NORMAL, LIGHT, DEPTH, SHADOW_MAP)]
		_GB("G-Buffer Selecter", Float) = 0
	}
	SubShader
	{
		Cull Off ZWrite Off ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			#pragma shader_feature _GB_DIFFUSE _GB_SPECULAR _GB_WORLD_SPACE_NORMAL _GB_LIGHT _GB_DEPTH _GB_SHADOW_MAP

			#define SHADOWS_SCREEN

			#include "UnityCG.cginc"

			#include "UnityDeferredLibrary.cginc"

			sampler2D _CameraGBufferTexture0;
			sampler2D _CameraGBufferTexture1;
			sampler2D _CameraGBufferTexture2;
			sampler2D _CameraGBufferTexture3;

			fixed4 frag (v2f_img i) : SV_Target
			{
#ifdef _GB_DIFFUSE
				return tex2D(_CameraGBufferTexture0, i.uv);
#elif _GB_SPECULAR
				return tex2D(_CameraGBufferTexture1, i.uv);
#elif _GB_WORLD_SPACE_NORMAL
				return tex2D(_CameraGBufferTexture2, i.uv);
#elif _GB_LIGHT
				return tex2D(_CameraGBufferTexture3, i.uv);
#elif _GB_DEPTH
				return Linear01Depth(tex2D(_CameraDepthTexture, i.uv).r);
#else
				return tex2D(_ShadowMapTexture, i.uv);
#endif
			}
			ENDCG
		}
	}
}
