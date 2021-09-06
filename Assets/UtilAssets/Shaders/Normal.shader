Shader "OUL/Normal"
{
	Properties
	{
		_MainTex("Texture", 2D) = "white" {}
	}

	SubShader
	{
		//Tags { "LightMode"="ForwardBase" }
		//LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"				// 行列変換関数(UnityObjectTo~)等の便利関数を使うときに必要
			#include "UnityLightingCommon.cginc"	// ライティング関係の関数を使うときに必要

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
				float4 tangent : TANGENT;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				fixed4 diffuseLight : COLOR0;
				float4 pos : SV_POSITION;
			};

			sampler2D _MainTex;
			float4 _MainTex_ST;

			v2f vert(appdata_base v)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(v.vertex);	// mul(MVP_MATRIX, v.vertex);
				float3 worldNormal = UnityObjectToWorldNormal(v.normal);
				half nl = max(0, dot(worldNormal, _WorldSpaceLightPos0.xyz));
				o.diffuseLight = nl * _LightColor0;
				o.diffuseLight.xyz += ShadeSH9(half4(worldNormal,1));
				//o.diffuseLight.xyz = worldNormal * 0.5 + 0.5;

				// 自分で作ったappdataの場合
				//o.uv = TRANSFORM_TEX(v.uv, _MainTex);

				// appdata_baseの場合
				o.uv = v.texcoord;

				// 深度情報
				//UNITY_TRANSFER_DEPTH(o.uv);
				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col = tex2D(_MainTex, i.uv);
				col = i.diffuseLight;
				return col;

				// 深度情報
				//UNITY_OUTPUT_DEPTH(i.uv);
			}
			ENDCG
		}

		// 影
		Pass
		{
			Tags{"LightMode" = "ShadowCaster"}

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_shadowcaster
			#include "UnityCG.cginc"

			struct v2f
			{
				V2F_SHADOW_CASTER;
			};

			v2f vert(appdata_base v)
			{
				v2f o;
				TRANSFER_SHADOW_CASTER_NORMALOFFSET(o);
				return o;
			}

			float4 frag(v2f i) : SV_Target
			{
				SHADOW_CASTER_FRAGMENT(i);
			}
			ENDCG
		}
	}
}
