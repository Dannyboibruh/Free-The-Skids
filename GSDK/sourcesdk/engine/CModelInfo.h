#define STUDIO_NONE						0x00000000
#define STUDIO_RENDER					0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS		0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS					0x00000008
#define STUDIO_STATIC_LIGHTING			0x00000010
#define STUDIO_WIREFRAME				0x00000020
#define STUDIO_ITEM_BLINK				0x00000040
#define STUDIO_NOSHADOWS				0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE		0x00000100

// Not a studio flag, but used to flag when we want studio stats
#define STUDIO_GENERATE_STATS			0x01000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SSAODEPTHTEXTURE			0x08000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SHADOWDEPTHTEXTURE		0x40000000

// Not a studio flag, but used to flag model as a non-sorting brush model
#define STUDIO_TRANSPARENCY	0x80000000
typedef unsigned short MDLHandle_t;
class CPhysCollide;
enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};
struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide	**solids;
	char			*pKeyValues;
};
class studiohdr_t;
class model_t;
class IMaterial;
class CUtlBuffer;
class IClientRenderable;
struct virtualmodel_t;
struct trace_t;
class IModelLoadCallback
{
public:
	virtual void OnModelLoadComplete(const model_t* pModel) = 0;

protected:
	// Protected destructor so that nobody tries to delete via this interface.
	// Automatically unregisters if the callback is destroyed while still pending.
	~IModelLoadCallback();
};
class CModelInfo
{
public:
	virtual							~CModelInfo(void) = 0;

	// Returns model_t* pointer for a model given a precached or dynamic model index.
	virtual const model_t			*GetModel(int modelindex) = 0;

	// Returns index of model by name for precached or known dynamic models.
	// Does not adjust reference count for dynamic models.
	virtual int						GetModelIndex(const char *name) const = 0;

	// Returns name of model
	virtual const char				*GetModelName(const model_t *model) const = 0;
	virtual vcollide_t				*GetVCollide(const model_t *model) = 0;
	virtual vcollide_t				*GetVCollide(int modelindex) = 0;
	virtual void					GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t *model) const = 0;
	virtual int						GetModelType(const model_t *model) const = 0;
	virtual void					*GetModelExtraData(const model_t *model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void					RecomputeTranslucency(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable, float fInstanceAlphaModulate = 1.0f) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual void					GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial) = 0;
	virtual bool					IsModelVertexLit(const model_t *model) const = 0;
	virtual const char				*GetModelKeyValueText(const model_t *model) = 0;
	virtual bool					GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius(const model_t *model) = 0;

	virtual const studiohdr_t		*FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t		*FindModel(void *cache) const = 0;
	virtual	virtualmodel_t			*GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual byte					*GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin,
		QAngle const& angles, trace_t* pTrace,
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin,
		QAngle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) = 0;
	virtual studiohdr_t				*GetStudiomodel(const model_t *mod) = 0;
	virtual int						GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int						GetModelSpriteHeight(const model_t *model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void					GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char			ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int						GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide			*GetCollideForVirtualTerrain(int index) = 0;

	virtual bool					IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;

	// Obsolete methods. These are left in to maintain binary compatibility with clients using the IVModelInfo old version.
	virtual const model_t			*FindOrLoadModel(const char *name) = 0;
	virtual void					InitDynamicModels() = 0;
	virtual void					ShutdownDynamicModels() = 0;
	virtual void					AddDynamicModel(const char *name, int nModelIndex = -1) = 0;
	virtual void					ReferenceModel(int modelindex) = 0;
	virtual void					UnreferenceModel(int modelindex) = 0;
	virtual void					CleanupDynamicModels(bool bForce = false) = 0;

	virtual MDLHandle_t				GetCacheHandle(const model_t *model) const = 0;

	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void					GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;

	// Poked by engine host system
	virtual void					OnLevelChange() = 0;

	virtual int						GetModelClientSideIndex(const char *name) const = 0;

	// Returns index of model by name, dynamically registered if not already known.
	virtual int						RegisterDynamicModel(const char *name, bool bClientSide) = 0;

	virtual bool					IsDynamicModelLoading(int modelIndex) = 0;

	virtual void					AddRefDynamicModel(int modelIndex) = 0;
	virtual void					ReleaseDynamicModel(int modelIndex) = 0;

	// Registers callback for when dynamic model has finished loading.
	// Automatically adds reference, pair with ReleaseDynamicModel.
	virtual bool					RegisterModelLoadCallback(int modelindex, IModelLoadCallback* pCallback, bool bCallImmediatelyIfLoaded = true) = 0;
	virtual void					UnregisterModelLoadCallback(int modelindex, IModelLoadCallback* pCallback) = 0;
};